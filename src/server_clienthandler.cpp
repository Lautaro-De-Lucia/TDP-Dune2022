
#include "server_clienthandler.h"

ClientHandler::ClientHandler(int init_energy, int init_spice ,Socket && client_socket,GameResources * game)
    :
    spice(init_spice),
    energy(init_energy),
    finished(false),
    game{game},
    thread(&ClientHandler::run,this,std::move(client_socket))
{
    this->faction = (player_t) -1;

	for (unit_t UNIT : units)
        this->units_to_create[UNIT] = 0,
        this->unit_time[UNIT] = 0,
        this->unit_creation_time[UNIT] = 2;

}

bool ClientHandler::isDone(){
    return this->finished;
}

void ClientHandler::close(){
    this->thread.join();
}

void ClientHandler::run(Socket && client_socket) {

    // faction setting
    int _faction;
    this->protocol.receive_faction_request(_faction, client_socket);
    this->faction = (player_t) _faction;

    if(this->faction == ATREIDES)
        this->game->createBuilding(this->faction,CONSTRUCTION_YARD,ATREIDES_INIT_POS_X,ATREIDES_INIT_POS_Y,this->spice, this->energy); 
    if(this->faction == HARKONNEN)
        this->game->createBuilding(this->faction,CONSTRUCTION_YARD,HARKONNEN_INIT_POS_X,HARKONNEN_INIT_POS_Y,this->spice, this->energy); 
    if(this->faction == ORDOS)
        this->game->createBuilding(this->faction,CONSTRUCTION_YARD,ORDOS_INIT_POS_X,ORDOS_INIT_POS_Y,this->spice, this->energy); 

    auto base_time_instruction = clock();

    while (true) {

        if(this->game->hasLost(this->faction)){
            this->finished = true;
            break;
        }

        command_t command;
        this->protocol.receive_command(command, client_socket);
        response_t res;
        if(command == CLOSE){
            this->finished = true;
            break;
        }

        int type, pos_x, pos_y, pos_x_min,pos_x_max,pos_y_min,pos_y_max;

        switch (command){
            case CREATE_UNIT:
                this->protocol.receive_create_unit_request(type, client_socket);
                res = queueUnit((unit_t)type);
                break;
            case CREATE_BUILDING:
                this->protocol.receive_create_building_request(type, pos_x, pos_y, client_socket);
                res = createBuilding(type, pos_x, pos_y);
                break;
            case MOUSE_LEFT_CLICK:
                this->protocol.receive_mouse_left_click(pos_x, pos_y, client_socket);
                handleLeftClick(pos_x, pos_y);
                res = RES_SUCCESS;
                break;
            case MOUSE_RIGHT_CLICK:
                this->protocol.receive_mouse_right_click(pos_x, pos_y, client_socket);
                handleRightClick(pos_x, pos_y);
                res = RES_SUCCESS;
                break;
            case MOUSE_SELECTION:
                this->protocol.receive_mouse_selection(pos_x_min, pos_x_max, pos_y_min, pos_y_max, client_socket);
                handleSelection(pos_x_min, pos_x_max, pos_y_min, pos_y_max);
                res = RES_SUCCESS;
                break;
            case IDLE:
                res = RES_SUCCESS;
                break;
            default:
                res = RES_SUCCESS;
                break;
        }
        
        this->responses_buffer.push_back(res);
        
        this->update();
        
        this->protocol.send_responses_size(this->responses_buffer.size(), client_socket);
        for (response_t res : this->responses_buffer)
            this->protocol.send_response(res, client_socket);
        
        this->responses_buffer.clear();
        
        reportState(client_socket);
    }
}

response_t ClientHandler::createUnit(int type, int& spice) {
    return this->game->createUnit(this->faction,(unit_t)type,spice);
}

response_t ClientHandler::createBuilding(int type, int pos_x, int pos_y) {    
    return this->game->createBuilding(this->faction,(building_t) type,pos_x,pos_y,this->spice,this->energy); 
}

void ClientHandler::handleLeftClick(int x, int y) {
    this->game->selectElement(this->faction,x,y);
}

void ClientHandler::handleSelection(int xmin, int xmax, int ymin, int ymax) {
    this->game->selectElements(this->faction,xmin,xmax,ymin,ymax);
}

void ClientHandler::handleRightClick(int x, int y) {
    this->game->reactToPosition(this->faction,x,y);
}

void ClientHandler::reportState(Socket& client_socket){
    //  Sending spice & energy state
    int max_spice = 20000 + this->game->getSpiceCapacity(this->faction);
    if(this->spice >= max_spice)
        this->spice = max_spice;
    this->protocol.send_player_state(this->spice,max_spice,this->energy,client_socket);
    //  Sending board state
    this->protocol.send_sand_cells_size(this->game->getTotalChangedCells(),client_socket);
    for(Position pos : this->game->getChangedCells()){
        this->protocol.send_sand_cell(pos.x,pos.y,this->game->getCell(pos.x,pos.y).getSpice(),client_socket);
    }    
    this->game->clearChangedCells();
    //  Sending elements states
    this->protocol.send_selectables_to_read(this->game->totalElements(),client_socket);
    this->game->sendElements(this->protocol,client_socket);
}

void ClientHandler::update(){    
	for (unit_t UNIT : units) 
        this->responses_buffer.push_back(this->checkCreation(UNIT));
    this->game->update();
}	

response_t ClientHandler::queueUnit(unit_t type){
    if(this->game->getCreator(this->faction,type) == -1)
		return RES_CREATE_UNIT_FAILURE_CREATOR;
    if(this->game->isEnabled(this->faction,type) == false)
        return RES_CREATE_UNIT_FAILURE_SPECIAL;
	this->units_to_create[type]++;
	return RES_SUCCESS;
}

response_t ClientHandler::checkCreation(unit_t type){
	if(this->units_to_create[type] == 0)
		return RES_SUCCESS;
    this->unit_time[type] += this->game->getTotalCreators(this->faction,type); 
    if(this->unit_time[type] >= this->unit_creation_time[type]){
		response_t res;
        res = this->game->createUnit(this->faction,type,this->spice);
        if (res != RES_CREATE_UNIT_SUCCESS){
            return res;
        } else {   
            this->unit_time[type] = 0;
            this->units_to_create[type]--;
            return res;
        }
    }   
    return RES_SUCCESS;
}
