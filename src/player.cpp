#include "player.h"

#include "astar.h"
#include <unistd.h>

extern std::map<color_t,SDL_Color> colors;

bool game_has_not_started  = true;

std::vector<std::string> file_input;

Player::Player(player_t faction, int spice, int c_spice, int energy, int c_energy,std::vector<std::vector<cell_t>> cells, CPlayer& client_player) 
: 
cplayer(client_player),
board(cells,this->elements)
{
    this->ID = 0;
    this->faction = faction;
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    this->c_energy = c_energy;
    this->efficiency = 1;
}

void Player::print(){
    std::cout << "Data for Player" << std::endl;
    std::cout << "spice: " << this->spice << std::endl;
    std::cout << "total spice capacity: " << this-> c_spice<< std::endl;
    std::cout << "energy: " << this->energy << std::endl;
    std::cout << "total energy capacity: " << this->c_energy << std::endl;
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

void Player::run() {

    auto base_time_instruction = clock();
    std::vector<int> action;

    while (true) {

        auto current_time = clock();
		auto frame_time_instruction = current_time - base_time_instruction;

        //  Load instructions from client  
        action.clear();  
        this->cplayer.clientUpdate(action);
        if(action.size() > 0)
            this->mouse_events.push(action);    
        
        if(frame_time_instruction > 100000 || game_has_not_started) {

            game_has_not_started = false;
            base_time_instruction = current_time;

            if (this->mouse_events.size() > 0) {
                std::vector<int> new_event = this->mouse_events.front();
                this->mouse_events.pop();
                command_t command = (command_t)(new_event[0]);
                switch (command){
                    case CREATE_UNIT:
                        createUnit(new_event[1]);
                        break;
                    case CREATE_BUILDING:
                        createBuilding(new_event[1], new_event[2], new_event[3]);
                        break;
                    case MOUSE_LEFT_CLICK:
                        handleLeftClick(new_event[1],new_event[2]);
                        break;
                    case MOUSE_RIGHT_CLICK:
                        handleRightClick(new_event[1],new_event[2]);
                        break;
                    case MOUSE_SELECTION:
                        handleSelection(new_event[1],new_event[2],new_event[3],new_event[4]);
                        break;
                    default:
                        break;
                }
            }
            update();
        }
        reportStateToClient();
    }
}

void Player::createBuilding(int type, int pos_x, int pos_y) {    
    //  Manufacture the building
    player_t building_faction = HARKONNEN;
    if(type == REFINERY)
        building_faction = ATREIDES;
    std::unique_ptr<Building> building = BuildingFactory::manufacture((building_t) type, building_faction,ID);
    //  Attempt to add to board
    if ((*building).place(board,pos_x,pos_y,this->spice,this->c_spice,this->energy,this->c_energy)){
        (this->elements).insert({ID, std::move(building)});
        State state;
        (this->elements)[ID]->getState(state);
        state.ID = ID;
        ID++;
        (this->cplayer).addElement((building_t) type, state);
        this->cplayer.print("Building succesfully created!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[YELLOW],1000);
        return;
    }
    this->cplayer.print("You can't build here!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
}

// SIGSEGV en algun Map::at() de esta función
void Player::createUnit(int type){
    //  Check if creator exists
    if (this->board.getCreator((unit_t) type) == -1) {
        this->cplayer.print("No creator for this unit right now",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
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
    if ((*unit).place(board,positions,&(this->spice))){
        (this->elements).insert({ID, std::move(unit)});
        State state;
        (this->elements)[ID]->getState(state);
        state.ID = ID;
        ID++;
        (this->cplayer).addElement((unit_t) type, state);        
        this->cplayer.print("Unit succesfully created",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[YELLOW],1000);
        return;
    }
    this->cplayer.print("There's no space to build this unit!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
}

void Player::handleLeftClick(int x, int y){
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

void Player::handleSelection(int xmin, int xmax, int ymin, int ymax){
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

void Player::handleRightClick(int x, int y){
    //  Get positions
    int pos_x = x;
    int pos_y = y;
    //  Traverse elements and make each selected unit handle the cell
    for (auto& e : this->elements){
        if (e.second->isSelected()){
            std::cout << e.first << ": ";
            e.second->react(pos_x,pos_y,board);
        }
    }
}

void Player::reportStateToClient(){
    State state;
    this->states.clear();
    for (auto& e : this->elements){
        state.ID = e.first;
        e.second->getState(state);
        states.push_back(state);
    }
    (this->cplayer).update(this->states,this->spice,this->energy);
}

void Player::update(){
    State state;
    std::vector<State> states;
    for (auto& e : this->elements){
        state.ID = e.first;
        e.second->update(state,this->board);
        states.push_back(state);
    }
}
