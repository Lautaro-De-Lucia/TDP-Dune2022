#include "server.h"

#include "astar.h"
#include <unistd.h>

extern std::map<color_t,SDL_Color> colors;

int ID = 0;

bool game_has_not_started  = true;

std::vector<std::string> file_input;

Server::Server(std::vector<std::vector<cell_t>> cells) 
: 
board(cells,this->elements)
{

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


// Player::Player(player_t faction, int spice, int c_spice, int energy, int c_energy,std::vector<std::vector<cell_t>> cells, CPlayer& client_player) 
void Server::run(player_t _faction, int _spice, int _c_spice, int _energy, int _c_energy, CPlayer& _client_player) {

    player_t faction = _faction;
    int spice = _spice;
    int c_spice = _c_spice;
    int energy = _energy;
    int c_energy = _c_energy;
    int efficiency = 1;

    CPlayer& client_player = _client_player;

    auto base_time_instruction = clock();
    std::vector<int> action;

    while (true) {

        auto current_time = clock();
		auto frame_time_instruction = current_time - base_time_instruction;

        //  Load instructions from client  
        action.clear();  
        client_player.clientUpdate(action);
        if(action.size() > 0)
            this->mouse_events.push(action);    
        
        if(frame_time_instruction > 10000 || game_has_not_started) {

            game_has_not_started = false;
            base_time_instruction = current_time;

            if (this->mouse_events.size() > 0) {
                std::vector<int> new_event = this->mouse_events.front();
                this->mouse_events.pop();
                
                // this->protocol.receive_command(command, client_socket);

                command_t command = (command_t)(new_event[0]);

                switch (command){
                    case CREATE_UNIT:
                        // this->protocol.receive_create_unit_request(type, client_socket);
                        createUnit(new_event[1], spice, client_player);
                        break;
                    case CREATE_BUILDING:
                        // this->protocol.receive_create_building_request(type, pos_x, pos_y, client_socket);
                        createBuilding(new_event[1], new_event[2], new_event[3], spice, c_spice, energy, c_energy, client_player);
                        break;
                    case MOUSE_LEFT_CLICK:
                        // this->protocol.receive_mouse_left_click(pos_x, pos_y, client_socket);
                        handleLeftClick(new_event[1],new_event[2], client_player);
                        break;
                    case MOUSE_RIGHT_CLICK:
                        // this->protocol.receive_mouse_right_click(pos_x, pos_y, client_socket);
                        handleRightClick(new_event[1],new_event[2], client_player);
                        break;
                    case MOUSE_SELECTION:
                        // this->protocol.receive_mouse_selection(pos_x_min, pos_x_max, pos_y_min, pos_y_max, client_socket);
                        handleSelection(new_event[1],new_event[2],new_event[3],new_event[4], client_player);
                        break;
                    case IDLE:
                        break;
                    default:
                        break;
                }
                //response_t res = RESPONSE_SUCCESS;
                //this->protocol.send_command_response(res, this->socket);
            }
            update();
        }
        reportStateToClient(client_player, spice, energy);
    }
}

void Server::createBuilding(int type, int pos_x, int pos_y, int& spice, int& c_spice, int& energy, int& c_energy, CPlayer& client_player) {    
    //  Manufacture the building
    player_t building_faction = HARKONNEN;
    if(type == REFINERY)
        building_faction = ATREIDES;
    std::unique_ptr<Building> building = BuildingFactory::manufacture((building_t) type, building_faction,ID);
    //  Attempt to add to board
    if ((*building).place(board,pos_x,pos_y,spice,c_spice,energy,c_energy)){
        (this->elements).insert({ID, std::move(building)});
        State state;
        (this->elements)[ID]->getState(state);
        state.ID = ID;
        ID++;
        client_player.addElement((building_t) type, state);
        client_player.print("Building succesfully created!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[YELLOW],1000);
        return;
    }
    client_player.print("You can't build here!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
}

// SIGSEGV en algun Map::at() de esta función
void Server::createUnit(int type, int& spice, CPlayer& client_player){
    //  Check if creator exists
    if (this->board.getCreator((unit_t) type) == -1) {
        client_player.print("No creator for this unit right now",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
        return;
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
    if ((*unit).place(board,positions,&spice)){
        (this->elements).insert({ID, std::move(unit)});
        State state;
        (this->elements)[ID]->getState(state);
        state.ID = ID;
        ID++;
        client_player.addElement((unit_t) type, state);        
        client_player.print("Unit succesfully created",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[YELLOW],1000);
        return;
    }
    client_player.print("There's no space to build this unit!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
}

void Server::handleLeftClick(int x, int y, CPlayer& client_player){
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
}

void Server::handleSelection(int xmin, int xmax, int ymin, int ymax, CPlayer& client_player){
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
}

void Server::handleRightClick(int x, int y, CPlayer& client_player){
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
}

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

//  void Server::reportBoardState()
void Server::update(){
    State state;
    std::vector<State> states;
    for (auto& e : this->elements){
        state.ID = e.first;
        e.second->update(state,this->board);
        states.push_back(state);
    }
}
