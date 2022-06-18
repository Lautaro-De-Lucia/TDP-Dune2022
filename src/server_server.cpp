#include "server_server.h"

#include "server_astar.h"
#include <unistd.h>

extern std::map<color_t,SDL_Color> colors;

int ID = 0;

bool game_has_not_started  = true;

std::vector<std::string> file_input;

//  @TODO: Meter en un bloque try{} en el main
Server::Server(const char* service_name, std::vector<std::vector<cell_t>> cells) 
:
socket(service_name),
client_socket(socket.accept()),
board(cells,this->elements)
{
    std::cout << "All good!" << std::endl;
}

void Server::print(){
    /*
    std::cout << "Data for Server" << std::endl;
    std::cout << "spice: " << this->spice << std::endl;
    std::cout << "total spice capacity: " << this-> c_spice<< std::endl;
    std::cout << "energy: " << this->energy << std::endl;
    std::cout << "total energy capacity: " << this->c_energy << std::endl;
    */
}

void printSeparator(){
    std::cout << "*****************************************************";
    std::cout << "*****************************************************";
    std::cout << "*****************************************************";
    std::cout << std::endl;
}

const char * cmddict [8] {
    "CREATING A BUILDING",
    "CREATING AN UNIT",
    "MAKING A CREATOR",
    "MAP CLICKED (LEFT)",
    "MAP CLICKED (RIGHT-CLICK)",
    "MAP SELECTED",
    "TEST ASTAR",
    "IDLE",
};


// Player::Player(player_t faction, int spice, int c_spice, int energy, int c_energy,std::vector<std::vector<cell_t>> cells) 
void Server::run(player_t _faction, int _spice, int _c_spice, int _energy, int _c_energy) {

    player_t faction = _faction;
    int spice = _spice;
    int c_spice = _c_spice;
    int energy = _energy;
    int c_energy = _c_energy;
    int efficiency = 1;

    auto base_time_instruction = clock();

    while (true) {

        auto current_time = clock();
		auto frame_time_instruction = current_time - base_time_instruction;

        if(frame_time_instruction > 10000 || game_has_not_started) {

            game_has_not_started = false;
            base_time_instruction = current_time;

            command_t command;
            std::cout << "Waiting for the command " << std::endl;
            this->protocol.receive_command(command, this->client_socket);
            std::cout << "Just got the command: " << command << std::endl;
            response_t res;

            int type;
            int pos_x;
            int pos_y;
            int pos_x_min;
            int pos_x_max;
            int pos_y_min;
            int pos_y_max;

            switch (command){
                
                case CREATE_UNIT:
                    this->protocol.receive_create_unit_request(type, this->client_socket);
                    res = createUnit(type, spice);
                    break;
                case CREATE_BUILDING:
                    this->protocol.receive_create_building_request(type, pos_x, pos_y, this->client_socket);
                    res = createBuilding(type, pos_x, pos_y, spice, c_spice, energy, c_energy);
                    break;
                case MOUSE_LEFT_CLICK:
                    this->protocol.receive_mouse_left_click(pos_x, pos_y, this->client_socket);
                    res = handleLeftClick(pos_x, pos_y);
                    break;
                case MOUSE_RIGHT_CLICK:
                    this->protocol.receive_mouse_right_click(pos_x, pos_y, this->client_socket);
                    res = handleRightClick(pos_x, pos_y);
                    break;
                case MOUSE_SELECTION:
                    this->protocol.receive_mouse_selection(pos_x_min, pos_x_max, pos_y_min, pos_y_max, this->client_socket);
                    res = handleSelection(pos_x_min, pos_x_max, pos_y_min, pos_y_max);
                    break;
                case IDLE:
                    res = RES_SUCCESS;
                    break;
                default:
                    res = RES_SUCCESS;
                    break;
            }
            this->protocol.send_command_response(res, this->client_socket);
            //update();
        }
        //reportStateToClient(spice, energy);
    }
}

response_t Server::createBuilding(int type, int pos_x, int pos_y, int& spice, int& c_spice, int& energy, int& c_energy) {    
    //  Manufacture the building
    player_t building_faction = HARKONNEN;
    if(type == REFINERY)
        building_faction = ATREIDES;
    std::unique_ptr<Building> building = BuildingFactory::manufacture((building_t) type, building_faction,ID);
    //  Attempt to add to board
    response_t res;
    res = (*building).place(board,pos_x,pos_y,spice,c_spice,energy,c_energy);
    if (res == RES_CREATE_BUILDING_SUCCESS){
        (this->elements).insert({ID, std::move(building)});
        State state;
        (this->elements)[ID]->getState(state);
        state.ID = ID;
        ID++;
        //client_player.addElement((building_t) type, state);
        //client_player.print("Building succesfully created!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[YELLOW],1000);
        return res;
    }
    //client_player.print("You can't build here!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
    return res;
}

// SIGSEGV en algun Map::at() de esta función
response_t Server::createUnit(int type, int& spice) {
    std::cout << "Creating a new Unit" << std::endl;
    std::cout << "type: " << type << std::endl;
    std::cout << "spice: " << spice << std::endl;
    //  Check if creator exists
    if (this->board.getCreator((unit_t) type) == -1) {
        return RES_CREATE_UNIT_FAILURE_CREATOR;
    }
    //  Get possible creating locations
    Position building_pos = (elements.at(board.getCreator((unit_t) type)))->getPosition();
    int building_dim_x = (elements.at(board.getCreator((unit_t) type)))->getDimX();
    int building_dim_y = (elements.at(board.getCreator((unit_t) type)))->getDimY();

    std::vector<Position> positions = board.getSurroundings(building_pos, building_dim_x, building_dim_y); //  FAILING HERE

    //  Create the unit
    player_t unit_faction = HARKONNEN;
    if(type == HARVESTER)
        unit_faction = ATREIDES;
    std::unique_ptr<Unit> unit = UnitFactory::create((unit_t) type,unit_faction,ID);
    //  Attempt adding it
    response_t res = (*unit).place(board,positions,&spice);
    if( res == RES_CREATE_UNIT_SUCCESS ){
        (this->elements).insert({ID, std::move(unit)});
        State state;
        (this->elements)[ID]->getState(state);
        state.ID = ID;
        ID++;
        //client_player.addElement((unit_t) type, state);        
        //client_player.print("Unit succesfully created",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[YELLOW],1000);
        return res;
    }
    return res;
}

response_t Server::handleLeftClick(int x, int y) {

    std::cout << "************************" << std::endl;
    std::cout << "Handling left click" << std::endl;
    std::cout << "position: (" << x << ", " << y << ")" << std::endl;
    
/*
    //  Get positions
    int pos_x = x;
    int pos_y = y;
    //  Leave selected only the units at that position
    for (auto& e : this->elements){
        if (e.second->contains(Position(pos_x,pos_y)))
            e.second->select();
        else
            e.second->unselect();
    }
*/
    return RES_SUCCESS;

}


response_t Server::handleSelection(int xmin, int xmax, int ymin, int ymax) {

    std::cout << "************************" << std::endl;
    std::cout << "Handling left click" << std::endl;
    std::cout << "min position: (" << xmin << ", " << ymin << ")" << std::endl;
    std::cout << "max position: (" << xmax << ", " << ymax << ")" << std::endl;

/*
    //  Get selection limits
    int Xmin = xmin;
    int Xmax = xmax;
    int Ymin = ymin;
    int Ymax = ymax;
    //  Traverse and mark as selected those that are included
    Area selection(Xmin,Xmax,Ymin,Ymax);
    for (auto& e : this->elements){
        e.second->unselect();
        if (e.second->isWithin(selection))
            e.second->select();
    }
*/
    return RES_SUCCESS;
}

response_t Server::handleRightClick(int x, int y) {

    std::cout << "************************" << std::endl;
    std::cout << "Handling right click" << std::endl;
    std::cout << "position: (" << x << ", " << y << ")" << std::endl;


    
/*    
    //  Get positions
    int pos_x = x;
    int pos_y = y;
    //  Traverse elements and make each selected unit handle the cell
    for (auto& e : this->elements){
        if (e.second->isSelected()){
            //std::cout << e.first << ": ";
            e.second->react(pos_x,pos_y,board);
        }
    }
    
*/
return RES_SUCCESS;
}
/*
void Server::reportStateToClient(CPlayer& client_player, int spice, int energy){
    //  Selectables state
    State state;
    this->states.clear();
    for (auto& e : this->elements){
        state.ID = e.first;
        //std::cout << state.ID << std::endl;
        e.second->getState(state);
        states.push_back(state);
    }
    client_player.update(this->states,spice,energy);
    //  Board state
        //  Enviar posiciones con arena y su cantidad de spice
        //  Enviar posiciones destruidas
        //  Limpiar posiciones destruidas
}
*/
//  void Server::reportBoardState()
/*
void Server::update(){
    State state;
    std::vector<State> states;
    for (auto& e : this->elements){
        state.ID = e.first;
        e.second->update(state,this->board);
        states.push_back(state);
    }
}
*/







