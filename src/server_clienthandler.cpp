
#include "server_clienthandler.h"

ClientHandler::ClientHandler(player_t faction,int init_energy, int init_spice ,Socket & client_socket,GameResources * game)
    :
    faction(faction),
    spice(init_spice),
    energy(init_energy),
    finished(false),
    game{game},
    thread(&ClientHandler::run,this,std::move(client_socket))
{}

bool ClientHandler::isDone(){
    return this->finished;
}

void ClientHandler::close(){
    this->thread.join();
}

void ClientHandler::run(Socket && client_socket) {

    auto base_time_instruction = clock();

    while (true) {

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
                res = createUnit(type, this->spice);
                break;
            case CREATE_BUILDING:
                this->protocol.receive_create_building_request(type, pos_x, pos_y, client_socket);
                res = createBuilding(type, pos_x, pos_y, this->spice, this->c_spice, this->energy, this->c_energy);
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
        this->protocol.send_command_response(res, client_socket);
        reportState(client_socket);
    }
}

response_t ClientHandler::createUnit(int type, int& spice) {
    return this->game->createUnit(this->faction,(unit_t)type,spice);
}

response_t ClientHandler::createBuilding(int type, int pos_x, int pos_y, int& spice, int& c_spice, int& energy, int& c_energy) {    
    return this->game->createBuilding(this->faction,(building_t) type,pos_x,pos_y,spice,c_spice,energy,c_energy); 
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
    this->protocol.send_player_state(this->spice,this->energy,client_socket);
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