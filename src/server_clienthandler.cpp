
#include "server_clienthandler.h"

ClientHandler::ClientHandler(int player_id, int init_energy, int init_spice, std::vector<bool> &ready, Socket &&client_socket, ThreadSafeQueue &tsq)
    : energy(init_energy),
      spice(init_spice),
      reading_flags(ready),
      player_socket(std::move(client_socket)),
      instruction_queue(tsq),
      finished(false),
      thread(&ClientHandler::run, this)
{
    this->player_id = player_id;
    this->faction = (player_t)-1;
    this->construction_wait = 0;

    this->finished = false;
    
}

bool ClientHandler::isDone()
{
    return this->finished;
}

void ClientHandler::close()
{
    this->finished = true;
    this->thread.join();
}

void ClientHandler::notifyGameStart()
{
    this->protocol.send_game_has_started(this->player_socket);
}

void ClientHandler::run2()
{
    // faction setting
    int _faction;
    this->protocol.receive_faction_request(_faction, this->player_socket);
    this->faction = (player_t)_faction;
    if (this->faction == ATREIDES)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, CONSTRUCTION_YARD, ATREIDES_INIT_POS_X, ATREIDES_INIT_POS_Y)));
    if (this->faction == HARKONNEN)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, CONSTRUCTION_YARD, HARKONNEN_INIT_POS_X, HARKONNEN_INIT_POS_Y)));
    if (this->faction == ORDOS)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, CONSTRUCTION_YARD, ORDOS_INIT_POS_X, ORDOS_INIT_POS_Y)));

    bool quit = false;
    while (true)
    {
        // std::cout << "I ain't done yet " << this->player_id << std::endl;
        while (this->reading_flags[this->player_id] == false)
        {
            if (finished == true)
            {
                quit = true;
                break;
            }
        }
        if (quit)
        {
            break;
        }
        // std::cout << "This should print" << this->player_id << std::endl;
        command_t command;
        // std::cout << "Receiving...: "<< this->player_id <<std::endl;
        this->protocol.receive_command(command, this->player_socket);

        int type, pos_x, pos_y, pos_x_min, pos_x_max, pos_y_min, pos_y_max;
        //  std::cout << "Pushing instruction to queue" << std::endl;
        switch (command)
        {
        case CREATE_UNIT:
            this->protocol.receive_create_unit_request(type, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<unit_create_t>(new unit_create_t(this->player_id, this->faction, (unit_t)type)));
            break;
        case CREATE_BUILDING:
            this->protocol.receive_create_building_request(type, pos_x, pos_y, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, type, pos_x, pos_y)));
            break;
        case MOUSE_LEFT_CLICK:
            this->protocol.receive_mouse_left_click(pos_x, pos_y, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<left_click_t>(new left_click_t(this->player_id, this->faction, pos_x, pos_y)));
            break;
        case MOUSE_RIGHT_CLICK:
            this->protocol.receive_mouse_right_click(pos_x, pos_y, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<right_click_t>(new right_click_t(this->player_id, this->faction, pos_x, pos_y)));
            break;
        case MOUSE_SELECTION:
            this->protocol.receive_mouse_selection(pos_x_min, pos_x_max, pos_y_min, pos_y_max, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<selection_t>(new selection_t(this->player_id, this->faction, pos_x_min, pos_x_max, pos_y_min, pos_y_max)));
            break;
        case IDLE:
            this->instruction_queue.push(std::unique_ptr<idle_t>(new idle_t(this->player_id, this->faction)));
            break;
        case CLOSE:
            this->instruction_queue.push(std::unique_ptr<close_t>(new close_t(this->player_id, this->faction)));
            break;
        default:
            break;
        }
        //  std::cout << "Pushed instruction to queue" << std::endl;
        this->reading_flags[this->player_id] = false;
    }
}
 
void ClientHandler::run(){
    
    int _faction;
    std::cout << "T1: Getting faction from client" << std::endl;
    this->protocol.receive_faction_request(_faction, this->player_socket);
    this->faction = (player_t)_faction;
    if (this->faction == ATREIDES)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, CONSTRUCTION_YARD, ATREIDES_INIT_POS_X, ATREIDES_INIT_POS_Y)));
    if (this->faction == HARKONNEN)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, CONSTRUCTION_YARD, HARKONNEN_INIT_POS_X, HARKONNEN_INIT_POS_Y)));
    if (this->faction == ORDOS)
        this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, CONSTRUCTION_YARD, ORDOS_INIT_POS_X, ORDOS_INIT_POS_Y)));
    
    while (true)
    {
        std::cout << "T1: Waiting for a command from client" << std::endl;
        command_t command;
        this->protocol.receive_command(command, this->player_socket);
        int type, pos_x, pos_y, pos_x_min, pos_x_max, pos_y_min, pos_y_max;
        switch (command)
        {
        case CREATE_UNIT:
            this->protocol.receive_create_unit_request(type, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<unit_create_t>(new unit_create_t(this->player_id, this->faction, (unit_t)type)));
            break;
        case CREATE_BUILDING:
            this->protocol.receive_create_building_request(type, pos_x, pos_y, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<building_create_t>(new building_create_t(this->player_id, this->faction, type, pos_x, pos_y)));
            break;
        case MOUSE_LEFT_CLICK:
            this->protocol.receive_mouse_left_click(pos_x, pos_y, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<left_click_t>(new left_click_t(this->player_id, this->faction, pos_x, pos_y)));
            break;
        case MOUSE_RIGHT_CLICK:
            this->protocol.receive_mouse_right_click(pos_x, pos_y, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<right_click_t>(new right_click_t(this->player_id, this->faction, pos_x, pos_y)));
            break;
        case MOUSE_SELECTION:
            this->protocol.receive_mouse_selection(pos_x_min, pos_x_max, pos_y_min, pos_y_max, this->player_socket);
            this->instruction_queue.push(std::unique_ptr<selection_t>(new selection_t(this->player_id, this->faction, pos_x_min, pos_x_max, pos_y_min, pos_y_max)));
            break;
        case IDLE:
            this->instruction_queue.push(std::unique_ptr<idle_t>(new idle_t(this->player_id, this->faction)));
            break;
        case CLOSE:
            this->instruction_queue.push(std::unique_ptr<close_t>(new close_t(this->player_id, this->faction)));
            break;
        default:
            break;
        }
    }
}


void ClientHandler::reportState(GameResources &game)
{
    //  Send responses loaded on the Server
    this->sendResponses();
    //  Sending spice & energy state
    int max_spice = INIT_CSPICE + game.getSpiceCapacity(this->faction);
    if (this->spice >= max_spice)
        this->spice = max_spice;
    std::cout << "Sending player energy: " << this->energy << std::endl;
    this->protocol.send_player_state(this->spice, max_spice, this->energy, this->player_socket);
    //  Sending board state
    this->protocol.send_sand_cells_size(game.getTotalChangedCells(), this->player_socket);
    for (Position pos : game.getChangedCells())
    {
        this->protocol.send_sand_cell(pos.x, pos.y, game.getCell(pos.x, pos.y).getSpice(), this->player_socket);
    }
    game.clearChangedCells();
    //  Sending creation data
    //  Sending elements states
    protocol.send_selectables_to_read(game.totalElements(),this->player_socket);
    game.sendElements(this->protocol, this->player_socket);

    this->sendCreationData();
    //  Sending creators
    int barrack_id = game.getCreator(this->faction, FREMEN);
    int light_factory_id = game.getCreator(this->faction, TRIKE);
    int heavy_factory_id = game.getCreator(this->faction, TANK);
    this->protocol.send_creators(barrack_id, light_factory_id, heavy_factory_id, this->player_socket);
}

void ClientHandler::sendResponses()
{
    this->protocol.send_responses_size(this->responses_buffer.size(), this->player_socket);
    for (response_t res : this->responses_buffer)
        this->protocol.send_response(res, this->player_socket);
    this->responses_buffer.clear();
}

void ClientHandler::setCreationData(std::vector<creation_t> &creation_data)
{
    this->creation_data = creation_data;
}

void ClientHandler::sendCreationData()
{
    this->protocol.send_creation_data_size(this->creation_data.size(), this->player_socket);
    for (creation_t c : this->creation_data)
        this->protocol.send_creation_data(c.creator_ID, c.unit_being_created, c.current_time, c.total_time, this->player_socket);
    this->creation_data.clear();
}

int &ClientHandler::getSpice()
{
    return this->spice;
}

int &ClientHandler::getConstructionWait()
{
    return this->construction_wait;
}

int ClientHandler::getID()
{
    return this->player_id;
}

player_t ClientHandler::getFaction()
{
    return this->faction;
}

int &ClientHandler::getEnergy()
{
    return this->energy;
}

void ClientHandler::setSpice(int spice)
{
    this->spice = spice;
}

void ClientHandler::setEnergy(int energy)
{
    this->energy = energy;
}

void ClientHandler::pushResponse(response_t response)
{
    this->responses_buffer.push_back(response);
}
