#include "player.h"

#include "astar.h"
#include <unistd.h>

extern std::map<color_t,SDL_Color> colors;

bool game_has_not_started  = true;

std::vector<std::string> file_input;

Player::Player(player_t faction, int spice, int c_spice, int energy, int c_energy,Board& shared_board, CPlayer& client_player) 
: 
cplayer(client_player),
board(shared_board)
{
    this->ID = 0;
    this->faction = faction;
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    this->c_energy = c_energy;
    this->efficiency = 1;
    this->creators.insert({HARVESTER,-1});
    this->creators.insert({TRIKE,-1});
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
    std::cout << "*****************************************************" << std::endl;
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
                    case MAKE_CREATOR:
                        //makeCreator(action[1]);
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
    std::unique_ptr<Building> building = BuildingFactory::manufacture((building_t) type, this->faction);
    //  Attempt to add to board
    if ((*building).place(board,pos_x,pos_y,this->spice,this->c_spice,this->energy,this->c_energy)){
        (this->elements).insert({ID++, std::move(building)});
        State state;
        (this->elements)[ID-1]->getState(state);
        state.ID = ID-1;
        (this->cplayer).addElement((building_t) type, state);
        this->makeCreator(state.ID);
        this->cplayer.print("Building succesfully created!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[YELLOW],1000);
        return;
    }
    this->cplayer.print("You can't build here!",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
}

void Player::createUnit(int type){
    //  Check if creator exists
    if (creators.at((unit_t) type) == -1) {
        this->cplayer.print("No creator for this unit right now",DATA_PATH FONT_IMPACT_PATH,200,300,10,colors[RED],1000);
        return;
    }
    //  Get possible creating locations
    std::vector<Position> positions = elements.at(creators.at((unit_t) type))->getSurroundings(); //  FAILING HERE
    //  Create the unit
    std::unique_ptr<Unit> unit = UnitFactory::create((unit_t) type,this->faction);
    //  Attempt adding it
    if ((*unit).place(board,positions,this->spice)){
        (this->elements).insert({ID++, std::move(unit)});
        State state;
        (this->elements)[ID-1]->getState(state);
        state.ID = ID-1;
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

void Player::updateMovables(){
    State state;
    for (auto& e : this->elements){
        if (e.second->moves()) {
            std::vector<Position>& path = e.second->get_remaining_path();  
            if (path.size() > 1) {
                e.second->getState(state);
                Position current_position(state.position);
                Position next_position = path.back();
                if (board.getCell(next_position.x, next_position.y).isOccupied()) {
                    Position end_position = path.front();
                    if (board.getCell(end_position.x, end_position.y).isOccupied()) {
                        std::vector<Position> neighbors = board.get_unoccupied_neighbors_of(end_position);
                        if (neighbors.size() == 0) {
                            // TODO: fix this border case
                            std::cerr << "Can't go there, this unit stops here" << std::endl;
                            continue;
                        }
                        // candidates for end_position
                        Position end_position_candidate = neighbors.back();
                        neighbors.pop_back();
                        // unit stops here
                        if (neighbors.size() == 0 && end_position_candidate == current_position)
                            continue;                        
                        if (end_position_candidate == current_position) {
                            end_position_candidate = neighbors.back();
                            neighbors.pop_back();
                        }
                        end_position = end_position_candidate;                                                
                    }
                    // para esta altura ya defini un end_position válido
                    aStar aStar;
                    std::vector<Position> new_path = aStar.algorithm(current_position, end_position, board);
                    // removing current_position from new_path
                    new_path.pop_back();
                    // now path is new_path
                    path = new_path;
                }
                next_position = path.back();
                path.pop_back();
                e.second->setPosition(next_position);
                board.move_unit(current_position, next_position, e.second->getFaction());
            }
        }
    }
}

void Player::makeCreator(int building_ID){
    if (this->elements.at(building_ID)->getName() == "Refinery")
        this->creators[HARVESTER] = building_ID; 
    if (this->elements.at(building_ID)->getName() == "Barrack"){
        this->creators[TRIKE] = building_ID;     
    }
    std::cout << this->elements.at(building_ID)->getName() << " of ID: " << building_ID << " is now a creator" << std::endl;
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