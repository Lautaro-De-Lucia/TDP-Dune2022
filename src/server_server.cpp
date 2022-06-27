#include "./server_server.h"

extern std::map<color_t,SDL_Color> colors;

#define MAX_CONNECTIONS 1

int ID = 0;

std::vector<std::string> file_input;

Server::Server(const char* service_name, std::vector<std::vector<cell_t>> cells) 
: 
socket(service_name),
game(cells),
TSQ(1) 
{
    this->running = false;
    for(player_t FACTION: factions)
        for (unit_t UNIT : units)
            this->units_to_create[FACTION][UNIT] = 0,
            this->unit_time[FACTION][UNIT] = 0,
            this->unit_creation_time[FACTION][UNIT] = 100;
}

void Server::acceptPlayers() {
    this->running = true;
    size_t i = 0;
    while (running == true){
        if(this->players.size() == 1)
            break;
        Socket client_socket = (this->socket.accept());
        this->ready_flags.push_back(true);
        this->players.push_back(std::unique_ptr<ClientHandler>(new ClientHandler(i,INIT_ENERGY,INIT_SPICE,this->ready_flags,std::move(client_socket),this->TSQ)));
        i++;
        //checkForFinishedClients();
    }
    for (size_t i = 0; i < this->players.size(); i++) {
        this->players[i]->notifyGameStart();
    }
    
    //closeAllClients();
}

void Server::checkForFinishedClients() {
    for (size_t k = 0; k < this->players.size(); k++){ 
        if (this->players[k]->isDone() == true){
            this->players[k]->close();
            this->players.erase(this->players.begin() + k);
        }
    }
}

void Server::closeAllClients() {
    for (size_t k = 0; k < this->players.size(); k++){ 
        this->players[k]->close();
    }
}

// @TODO: lanzar excepción
void Server::stop() {
    running = false;
    if ((this->socket).shutdown(SHUT_RDWR))
        std::cerr << "SHUTDOWN_ERROR: " << '\n';
}

void Server::run() {
    while (true) {
        this->checkForLosingPlayers();
        while(this->TSQ.getSize() < this->players.size()){}
        for(size_t i = 0 ; i < this->players.size(); i++) {
            std::unique_ptr<instruction_t> new_instruction = this->TSQ.pop();
            this->handleInstruction(new_instruction);
        }
        this->sendResponses();
        this->update();
        for(size_t i = 0; i < this->players.size(); i++)
            this->players[i]->reportState(this->game);
        this->enableReading();
    }
}

void Server::checkForLosingPlayers(){
    //std::cout << "Server::checkForLosingPlayers()" << std::endl;
    return;
}

void Server::handleInstruction(std::unique_ptr<instruction_t> & INS) {
    switch(INS->command){
        case CREATE_UNIT:
            this->handleInstruction(dynamic_cast<unit_create_t&>(*INS));
            break;
        case CREATE_BUILDING:
            this->handleInstruction(dynamic_cast<building_create_t&>(*INS));
            break; 
        case MOUSE_RIGHT_CLICK:
            this->handleInstruction(dynamic_cast<right_click_t&>(*INS));
            break;
        case MOUSE_LEFT_CLICK:
            this->handleInstruction(dynamic_cast<left_click_t&>(*INS));
            break;
        case MOUSE_SELECTION:
            this->handleInstruction(dynamic_cast<selection_t&>(*INS));
            break; 
        case IDLE:
            this->handleInstruction(dynamic_cast<idle_t&>(*INS));
            break;     
        default:
            break;
    }
}


void Server::handleInstruction(building_create_t & INS) {
    int current_spice = this->players[INS.player_ID]->getSpice();
    int current_energy = this->players[INS.player_ID]->getEnergy();
    this->responses[INS.player_ID].push_back(
        this->game.createBuilding(INS.faction,(building_t)INS.type,INS.pos_x,INS.pos_y,current_spice,current_energy)
    );
    this->players[INS.player_ID]->setSpice(current_spice);
    this->players[INS.player_ID]->setEnergy(current_energy);
}
/*
void Server::handleInstruction(unit_create_t & INS) {
    int current_spice = this->players[INS.player_ID]->getSpice();
    this->responses[INS.player_ID].push_back(
        this->game.createUnit(INS.faction,(unit_t)INS.type,current_spice)
    );
    this->players[INS.player_ID]->setSpice(current_spice);
}
*/

void Server::handleInstruction(unit_create_t & INS) {
    if(this->game.getCreator(INS.faction,INS.type) == -1){
		this->responses[INS.player_ID].push_back(RES_CREATE_UNIT_FAILURE_CREATOR);
        return;
    }
    if(this->game.isEnabled(INS.faction,INS.type) == false){
        this->responses[INS.player_ID].push_back(RES_CREATE_UNIT_FAILURE_SPECIAL);
        return;
    }
	this->units_to_create[INS.faction][INS.type]++;
	this->responses[INS.player_ID].push_back(RES_SUCCESS);
}

void Server::handleInstruction(left_click_t & INS) {
    this->game.selectElement(INS.faction,INS.pos_x,INS.pos_y);   
}

void Server::handleInstruction(right_click_t & INS) {
    this->game.reactToPosition(INS.faction,INS.pos_x,INS.pos_y);    
}

void Server::handleInstruction(selection_t & INS) {
    this->game.selectElements(INS.faction,INS.Xm,INS.XM,INS.Ym,INS.YM);    
}

void Server::handleInstruction(idle_t & INS) {}

void Server::sendResponses() {
    for (size_t i = 0 ; i < this->players.size(); i++)
        this->players[i]->sendResponses(this->responses[i]);
}

void Server::update(){    
    for(std::unique_ptr<ClientHandler>& player : this->players )
        for(unit_t UNIT: units)
            this->responses[player->getID()].push_back(checkCreation(player->getFaction(),UNIT));
    this->game.update();
}

std::unique_ptr<ClientHandler> & Server::getPlayer(player_t faction){
    for(size_t i = 0; i < this->players.size() ; i++)
        if(this->players[i]->getFaction()==faction)
            return this->players[i];
    throw std::runtime_error("Player of this faction has not been initialized");        
}

response_t Server::checkCreation(player_t faction, unit_t type){
    if(this->units_to_create[faction][type] == 0)
		return RES_SUCCESS;
    this->unit_time[faction][type] += this->game.getTotalCreators(faction,type); 
    if(this->unit_time[faction][type] >= this->unit_creation_time[faction][type]){
		response_t res;
        int spice = this->getPlayer(faction)->getSpice();
        res = this->game.createUnit(faction,type,spice);
        if (res != RES_CREATE_UNIT_SUCCESS){
            return res;
        } else {   
            this->unit_time[faction][type] = 0;
            this->units_to_create[faction][type]--;
            this->getPlayer(faction)->setSpice(spice);
            return res;
        }
    }   
    return RES_SUCCESS;
}

void Server::enableReading(){
    for(size_t i = 0; i < this->ready_flags.size() ; i++)
        this->ready_flags[i] = true;
}