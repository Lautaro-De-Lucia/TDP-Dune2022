#include "server_server.h"

extern std::map<color_t,SDL_Color> colors;

std::vector<std::string> file_input;

int ID = 0;

Server::Server(const char* service_name, std::vector<std::vector<cell_t>> cells) 
: 
socket(service_name),
game(cells),
TSQ(MAX_CONNECTIONS) 
{
    this->running = true;
    for(player_t FACTION: factions)
        for (unit_t UNIT : units)
            this->unit_time[FACTION][UNIT] = 0,
            this->unit_creation_time[FACTION][UNIT] = 50;
}

void Server::acceptPlayers() {
    this->running = true;
    size_t i = 0;
    while (running){
        if(this->players.size() == MAX_CONNECTIONS)
            break;
        Socket client_socket = (this->socket.accept());
        this->ready_flags.push_back(true);
        this->players.push_back(std::unique_ptr<ClientHandler>(new ClientHandler(i,INIT_ENERGY,INIT_SPICE,this->ready_flags,std::move(client_socket),this->TSQ)));
        i++;
        checkForFinishedClients();
    }
    if(this->players.size() == MAX_CONNECTIONS)
        for (size_t i = 0; i < this->players.size(); i++) 
            this->players[i]->notifyGameStart();
    if(this->players.size() < MAX_CONNECTIONS) {       
        closeAllClients();    
        throw std::runtime_error("Server closed unexpectedly");
    }
}

void Server::closeAllClients() {
    for (size_t k = 0; k < this->players.size(); k++)
        if(!this->players[k]->isDone())
            std::cout << "Closing client: " << k << std::endl,
            this->players[k]->close();
    //std::cout << "Done" << std::endl;
}

// @TODO: lanzar excepción
void Server::stopClientAccept() {
    //this->running = false;
    //if ((this->socket).shutdown(SHUT_RDWR))
        //std::cerr << "SHUTDOWN_ERROR: " << '\n';
}

void Server::stopGameLoop() {
    this->running = false;
}

void Server::read_command(std::istream& input_stream) {
    std::string command;
    while (1) {
        sleepms(100);
        input_stream >> command;
        if (command == "q")
            continue;
    }
}

void Server::run2() {
    while (this->running) {
        while(this->TSQ.getSize() < this->players.size()){}
        for(size_t i = 0 ; i < this->players.size(); i++) { 
            std::unique_ptr<instruction_t> new_instruction = this->TSQ.pop();
            this->handleInstruction(new_instruction);
        }
        //std::cout << "Sending responses" << std::endl;
        this->sendResponses();
        //std::cout << "Updating game" << std::endl;
        this->update();
        //std::cout << "Reporting the state of the game to players" << std::endl;
        this->reportCreationState();
        for(size_t i = 0; i < this->players.size(); i++)
            this->players[i]->reportState(this->game);
        //std::cout << "Enabling reading" << std::endl;
        this->enableReading();

        // mismo que los keepalive de los clientes, no mas keepalive
        std::vector<int> players_to_remove;
        for (size_t i = 0; i < (this->players).size(); i++) {
            if (this->players[i]->isDone()){
                std::cout << "Player: " << i << " is done" << std::endl;
                this->players[i]->close();
                players_to_remove.push_back(i);
            }
        }

        if (players_to_remove.size() != 0) {
            for (size_t i = players_to_remove.size() - 1; i >= 0; i--)
                this->players.erase(this->players.begin() + players_to_remove[i]);
        }

        players_to_remove.clear();
    }
    std::cout << "Game loop stopped. Closing all clients..." << std::endl;
    this->closeAllClients();
}

void Server::run() {
    while (this->running) {
        if(!this->TSQ.isEmpty()){
            std::cout << "T2: Receiving new instruction" << std::endl;
            std::unique_ptr<instruction_t> new_instruction = this->TSQ.pop();
            this->handleInstruction(new_instruction);
            this->sendResponses();
        }
        std::cout << "T2: Updating..." << std::endl;
        this->update();
        this->reportCreationState();
        for(size_t i = 0; i < this->players.size(); i++)
            this->players[i]->reportState(this->game);
        this->checkForFinishedClients();
        sleepms(100);
    }
    this->closeAllClients();
}

void Server::checkForFinishedClients() {
    std::vector<int> players_to_remove;
    for (size_t k = 0; k < (this->players).size(); k++){ 
        if (this->players[k]->isDone() == true){
            this->players[k]->close();
            players_to_remove.push_back(k);
        }
    }

    if (players_to_remove.size() == 0)
        return;

    for (size_t i = players_to_remove.size() - 1; i >= 0; i--)
        this->players.erase(this->players.begin() + players_to_remove[i]);
}

void Server::handleInstruction(std::unique_ptr<instruction_t> & INS) {
    switch(INS->command){
        case CREATE_UNIT:
            std::cout << "T2: creating new unit" << std::endl;
            this->handleInstruction(dynamic_cast<unit_create_t&>(*INS));
            break;
        case CREATE_BUILDING:
            std::cout << "T2: creating new building" << std::endl;
            this->handleInstruction(dynamic_cast<building_create_t&>(*INS));
            break; 
        case MOUSE_RIGHT_CLICK:
            std::cout << "T2: Mouse right click" << std::endl;
            this->handleInstruction(dynamic_cast<right_click_t&>(*INS));
            break;
        case MOUSE_LEFT_CLICK:
            std::cout << "T2: Mouse left click" << std::endl;
            this->handleInstruction(dynamic_cast<left_click_t&>(*INS));
            break;
        case MOUSE_SELECTION:
            std::cout << "T2: Mouse selection" << std::endl;
            this->handleInstruction(dynamic_cast<selection_t&>(*INS));
            break; 
        case IDLE:
            this->handleInstruction(dynamic_cast<idle_t&>(*INS));
            break;
        case CLOSE:
            this->handleInstruction(dynamic_cast<close_t&>(*INS));
            break; 
        default:
            break;
    }
}


void Server::handleInstruction(building_create_t & INS) {
    this->responses[INS.player_ID].push_back(
        this->game.createBuilding(INS.faction,(building_t)INS.type,INS.pos_x,INS.pos_y,this->players[INS.player_ID]->getSpice(),this->players[INS.player_ID]->getEnergy())
    );
}

void Server::handleInstruction(unit_create_t & INS) {

    if (this->game.getCreator(INS.faction,INS.type) == -1) {
		this->responses[INS.player_ID].push_back(RES_CREATE_UNIT_FAILURE_CREATOR);
        return;
    }
    if (this->game.isEnabled(INS.faction,INS.type) == false){
        this->responses[INS.player_ID].push_back(RES_CREATE_UNIT_FAILURE_SPECIAL);
        return;
    }
	this->creating_queues[INS.faction][getCreator(INS.type)].push(INS.type);
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

void Server::handleInstruction(close_t & INS) {
    if (this->players.size() > 1) {
        this->game.deleteElement(this->game.getElementAt(this->getFactionBase(INS.faction).x, this->getFactionBase(INS.faction).y)->getID());
    }

    int player_index = INS.player_ID;

    for (size_t i = 0; i < this->players.size(); i++)
        if (this->players[i]->getID() == INS.player_ID)
            player_index = i;

    this->players[player_index]->close();

    this->players.erase(this->players.begin() + player_index);
}

Position Server::getFactionBase(player_t faction) {

    Position pos(-1,-1);

    switch (faction)
    {
    case ATREIDES:
        pos = Position(ATREIDES_INIT_POS_X, ATREIDES_INIT_POS_Y);
        break;
    case ORDOS:
        pos = Position(ORDOS_INIT_POS_X, ORDOS_INIT_POS_Y);
        break;
    case HARKONNEN:
        pos = Position(HARKONNEN_INIT_POS_X, HARKONNEN_INIT_POS_Y);
        break;
    default:
        break;
    }

    return pos;
}

void Server::sendResponses() {
    for (size_t i = 0 ; i < this->players.size(); i++)
        this->players[i]->sendResponses(this->responses[i]);
}

void Server::update(){    
    this->game.update();
    for(std::unique_ptr<ClientHandler>& player : this->players){
        int & energy = player->getEnergy();
        energy = this->game.getTotalEnergy(player->getFaction());
        if(energy < 0)
            this->time_penalty[player->getFaction()] = 0,
            this->time_penalty[player->getFaction()] += round(abs(energy/10));
        else
            this->time_penalty[player->getFaction()] = 0;
        for(building_t CREATOR : creators)
            this->responses[player->getID()].push_back(checkCreation(player->getFaction(),CREATOR));
    }
}

std::unique_ptr<ClientHandler> & Server::getPlayer(player_t faction) {
    for(size_t i = 0; i < this->players.size() ; i++)
        if(this->players[i]->getFaction()==faction)
            return this->players[i];
    throw std::runtime_error("Player of this faction has not been initialized");        
}

response_t Server::checkCreation(player_t faction, building_t creator) {
    if(this->creating_queues[faction][creator].empty())
        return RES_SUCCESS;
    unit_t queued = this->creating_queues[faction][creator].front();
    this->unit_time[faction][queued] += this->game.getTotalCreators(faction,queued); 
    for(size_t i = 0; i < this->players.size(); i++)
        if(this->players[i]->getFaction() == faction)
            if(this->game.getCreator(faction,queued) == -1){
                while(!this->creating_queues[faction][creator].empty())
                    this->creating_queues[faction][creator].pop();
                this->unit_time[faction][queued] = 0;
            } else {
                this->creation_data[i].push_back(creation_t(
                    this->game.getCreator(faction,queued),
                    queued,
                    this->unit_time[faction][queued],
                    this->unit_creation_time[faction][queued]+this->time_penalty[faction])
                );
            }
    if(this->unit_time[faction][queued] >= this->unit_creation_time[faction][queued]+this->time_penalty[faction]){
        response_t res;
        res = this->game.createUnit(faction,queued,this->getPlayer(faction)->getSpice());
        if (res != RES_CREATE_UNIT_SUCCESS){
            return res;
        } else {   
            this->unit_time[faction][queued] = 0;
            this->creating_queues[faction][creator].pop();
            return res;
        }
    }   
    return RES_SUCCESS;
}

void Server::reportCreationState(){
    for (size_t i = 0 ; i < this->players.size(); i++)
        this->players[i]->setCreationData(this->creation_data[i]);
    for(size_t i = 0; i < this->creation_data.size() ; i++)
        this->creation_data[i].clear();
}

void Server::enableReading(){
    for(size_t i = 0; i < this->ready_flags.size() ; i++)
        this->ready_flags[i] = true;
}

building_t Server::getCreator(unit_t type){
    if(type == FREMEN || type == INFANTRY || type == SARDAUKAR)
        return BARRACK;
    if(type == TRIKE || type == HARVESTER)
        return LIGHT_FACTORY;
    if(type == TANK || type == DEVASTATOR)
        return HEAVY_FACTORY;
    throw std::runtime_error("No creator for this type");
}