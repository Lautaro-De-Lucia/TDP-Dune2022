
#include "server_clienthandler.h"

ClientHandler::ClientHandler(int player_id, int init_energy, int init_spice,std::vector<bool> & ready,Socket && client_socket,ThreadSafeQueue & tsq)
    :
    spice(init_spice),
    energy(init_energy),
    reading_flags(ready),
    instruction_queue(tsq),
    player_socket(std::move(client_socket)),
    finished(false),
    thread(&ClientHandler::run,this)
{
    this->player_id = player_id;
    this->faction = (player_t) -1;

	for (unit_t UNIT : units)
        this->units_to_create[UNIT] = 0,
        this->unit_time[UNIT] = 0,
        this->unit_creation_time[UNIT] = 2;

    this->finished = false;
}

bool ClientHandler::isDone(){
    return this->finished;
}

void ClientHandler::close(){
    this->thread.join();
}

void ClientHandler::notifyGameStart(){
    this->protocol.send_game_has_started(this->player_socket);
}

void ClientHandler::run() {

    // faction setting
    int _faction;
    this->protocol.receive_faction_request(_faction, this->player_socket);
    this->faction = (player_t) _faction;
    if(this->faction == ATREIDES)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction,CONSTRUCTION_YARD,ATREIDES_INIT_POS_X,ATREIDES_INIT_POS_Y)));
    if(this->faction == HARKONNEN)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction,CONSTRUCTION_YARD,HARKONNEN_INIT_POS_X,HARKONNEN_INIT_POS_Y)));
    if(this->faction == ORDOS)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction,CONSTRUCTION_YARD,ORDOS_INIT_POS_X,ORDOS_INIT_POS_Y)));

    while (true) {
        
        while(this->reading_flags[this->player_id] == false){}

        command_t command;
        this->protocol.receive_command(command, this->player_socket);

        if(command == CLOSE) {
            this->finished = true;
            break;
        }

        int type, pos_x, pos_y, pos_x_min,pos_x_max,pos_y_min,pos_y_max;
        //  std::cout << "Pushing instruction to queue" << std::endl;
        switch (command){
            case CREATE_UNIT:
                this->protocol.receive_create_unit_request(type, this->player_socket);
                this->instruction_queue.push(std::unique_ptr<unit_create_t>(new unit_create_t(this->player_id, this->faction,(unit_t)type)));
                break;
            case CREATE_BUILDING:
                this->protocol.receive_create_building_request(type, pos_x, pos_y, this->player_socket);
                this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction,type,pos_x,pos_y)));
                break;
            case MOUSE_LEFT_CLICK:
                this->protocol.receive_mouse_left_click(pos_x, pos_y, this->player_socket);
                this->instruction_queue.push(std::unique_ptr<left_click_t>(new left_click_t(this->player_id, this->faction,pos_x,pos_y)));
                break;
            case MOUSE_RIGHT_CLICK:
                this->protocol.receive_mouse_right_click(pos_x, pos_y, this->player_socket);
                this->instruction_queue.push(std::unique_ptr<right_click_t>(new right_click_t(this->player_id, this->faction,pos_x,pos_y)));
                break;
            case MOUSE_SELECTION:
                this->protocol.receive_mouse_selection(pos_x_min, pos_x_max, pos_y_min, pos_y_max, this->player_socket);
                this->instruction_queue.push(std::unique_ptr<selection_t>(new selection_t(this->player_id, this->faction,pos_x_min,pos_x_max,pos_y_min,pos_y_max)));
                break;
            case IDLE:
                this->instruction_queue.push(std::unique_ptr<idle_t>(new idle_t(this->player_id, this->faction)));
                break;
            default:
                break;
        }
        //  std::cout << "Pushed instruction to queue" << std::endl;
        this->reading_flags[this->player_id] = false;
    }
}

void ClientHandler::reportState(GameResources & game){
    //  Sending spice & energy state
    int max_spice = 20000 + game.getSpiceCapacity(this->faction);
    if (this->spice >= max_spice)
        this->spice = max_spice;
    this->protocol.send_player_state(this->spice,max_spice,this->energy,this->player_socket);
    //  Sending board state
    this->protocol.send_sand_cells_size(game.getTotalChangedCells(),this->player_socket);
    for(Position pos : game.getChangedCells()){
        this->protocol.send_sand_cell(pos.x,pos.y,game.getCell(pos.x,pos.y).getSpice(),this->player_socket);
    }    
    game.clearChangedCells();
    //  Sending elements states
    this->protocol.send_selectables_to_read(game.totalElements(),this->player_socket);
    game.sendElements(this->protocol,this->player_socket);
}

void ClientHandler::sendResponses(std::vector<response_t> & responses){    
    this->protocol.send_responses_size(responses.size(), this->player_socket);
    for (response_t res : responses)
        this->protocol.send_response(res, this->player_socket);
    responses.clear();
}

int & ClientHandler::getSpice() {
    return this->spice;
}

int ClientHandler::getID() {
    return this->player_id;
}

player_t ClientHandler::getFaction() {
    return this->faction;
}

int & ClientHandler::getEnergy() {
    return this->energy;
}

void ClientHandler::setSpice(int spice) {
    this->spice = spice;
}

void ClientHandler::setEnergy(int energy) {
    this->energy = energy;
}
