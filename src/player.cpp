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

    // @ignore
    TextFileHandler command_reader(DATA_PATH INSTRUCTIONS_FILE);
    auto base_time_instruction = clock();

    while (1) {

        auto current_time = clock();
		auto frame_time_instruction = current_time - base_time_instruction;
        //  
        std::vector<int> action;
        this->cplayer.clientUpdate(action);
        if(action.size() > 0){
            std::cout << "Client sent a new mouse action" << std::endl;
            this->mouse_events.push(action);    
        }
        reportState();

        if(frame_time_instruction > 100000 || game_has_not_started) {

            game_has_not_started = false;
            base_time_instruction = current_time;

            if (this->mouse_events.size() > 0) {
                std::cout << "Thus I am handling the event" << std::endl;
                std::vector<int> new_event = this->mouse_events.front();
                this->mouse_events.pop();
                std::cout << "The new event has values:" << std::endl;
                for (int value : new_event){
                    std::cout << " " << value;
                }
                std::cout << std::endl;
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
                    case IDLE:
                        handleIdle();
                        break;
                    default:
                        break;
                }
            }
            updateMovables();
        }
    }
}

/*
We only need building <type> and <position>.
Other parameters like the dimensions are lifted from
a text file (as specified in the assignment's instructions)
and will be stored in the Factory class.
(SEND)   
    SERVER <--------[TYPE](8)[POS_X](16)[ṔOS_Y](16)--------- CLIENT
(RECEIVE)
    CASE SUCCESS: 
        SERVER ------------[SUCCESS](8)--------------------> CLIENT
            //  Client is responsible for adding the new unit to it's selectable array                
            //  And this unit is responsable for rendering itself on the board
    CASE FAILURE:
        SERVER ------------[FAILURE](8)--------------------> CLIENT
            //  Client does nothing
*/

void Player::createBuilding(int type, int pos_x, int pos_y) {

    
    //  Manufacture the building
    std::unique_ptr<Building> building = BuildingFactory::manufacture((building_t) type, this->faction);

    //  Attempt to add to board
    (this->place)((*building),Position(pos_x, pos_y));

    if ((*building).place(board,pos_x,pos_y,this->spice,this->c_spice,this->energy,this->c_energy)){
        (this->elements).insert({ID++, std::move(building)});

        State state;
        (this->elements)[ID-1]->getState(state);
        state.ID = ID-1;
        (this->cplayer).addElement((building_t) type, state);
        this->makeCreator(state.ID);

        std::cout << "Building succesfully created" << std::endl;
        return;
    }
    std::cout << "Can't build here" << std::endl;
}

/*
We only need unit <type>
[TYPE](8)
*/
void Player::createUnit(int type){

    std::cout<< "Creating new unit" << std::endl;
    //  Attempt to add to board
    //  Create the unit

    if (creators.at((unit_t) type) == -1) {
        this->cplayer.print(
            "No creator for this unit right now",
            DATA_PATH FONT_IMPACT_PATH,
            200,
            300,
            10,
            colors[RED],
            1000);
        return;
    }

    std::vector<Position> positions = elements.at(creators.at((unit_t) type))->getSurroundings(); //  FAILING HERE
    
    std::unique_ptr<Unit> unit = UnitFactory::create((unit_t) type,this->faction);
    //  Attempt adding it
    //  REFACTOR: place should probably be a method of the Player class
        // place(Board& board,Building & building,Position& position)
        // place(Board& board,Unit & unit,std::vector<Positions> positions)

    if ((*unit).place(board,positions,this->spice)){
        (this->elements).insert({ID++, std::move(unit)});

        State state;
        (this->elements)[ID-1]->getState(state);
        state.ID = ID-1;
        (this->cplayer).addElement((unit_t) type, state);
        
        std::cout << "Unit succesfully created" << std::endl;
        return;
    }
    std::cout << "Can't build here" << std::endl;
}

/*
(SEND)   
    SERVER <---------------[POS_X](16)[ṔOS_Y](16)--------- CLIENT
(RECEIVE)
    CASE SUCCESS: 
        SERVER ------------[SUCCESS](8)--------------------> CLIENT
*/
void Player::handleLeftClick(int x, int y){
    std::cout << "Client just did a left click on the map" << std::endl;
    //  Get positions
    int pos_x = x;
    int pos_y = y;
    std::cout << "On position: " << Position(pos_x,pos_y) << std::endl;
    //  Leave selected only the units at that position
    for (auto& e : this->elements){
        if (e.second->contains(Position(pos_x,pos_y)))
            e.second->select();
        else
            e.second->unselect();
    }
    //  Notify success  
    std::cout << "All good" << std::endl;      
}
/*
(SEND)   
    SERVER <-----[Xmin](16)[Xmax](16)[Ymin](16)[Ymax](16)--------- CLIENT
(RECEIVE)
    CASE SUCCESS: 
        SERVER ------------[SUCCESS](8)--------------------> CLIENT
*/
void Player::handleSelection(int xmin, int xmax, int ymin, int ymax){
    std::cout << "Client just selected a part of the map" << std::endl;
    //  Get selection limits
    int Xmin = xmin;
    int Xmax = xmax;
    int Ymin = ymin;
    int Ymax = ymax;
    std::cout << "Selection: (" << Xmin << "," << Xmax << "," << Ymin << "," << Ymax << ")" << std::endl;
    //  Traverse and mark as selected those that are included
    Area selection(Xmin,Xmax,Ymin,Ymax);
    for (auto& e : this->elements){
        e.second->unselect();
        if (e.second->isWithin(selection))
            e.second->select();
    }
    //  notify success
    std::cout << "Selected units have been marked as selected!" << std::endl;
}
/*
(SEND)   
    SERVER <---------------[POS_X](16)[ṔOS_Y](16)--------- CLIENT
(RECEIVE)
    CASE SUCCESS: 
        SERVER ------------[SUCCESS](8)--------------------> CLIENT
*/
void Player::handleRightClick(int x, int y){
    std::cout << "Client just did a right click on the map" << std::endl;
    //  Get positions
    int pos_x = x;
    int pos_y = y;
    std::cout << "On position: " << Position(pos_x, pos_y) << std::endl;
    //  Traverse elements and make each selected unit handle the cell
    for (auto& e : this->elements){
        if (e.second->isSelected()){
            std::cout << e.first << ": ";
            e.second->react(pos_x,pos_y,board);
        }
    }
}

void Player::handleIdle(){
}

/*
(RECEIVE)
    CASE SUCCESS: 
        SERVER ------------>{...,[sel][pos_y][pos_x][LP][ID],...}[total]---------> CLIENT
*/


void Player::reportState(){

    /*
    std::cout << "Sending data to client" << std::endl;
    std::cout << "Total units to update: " << this->elements.size() << std::endl;
    std::cout << "|  ID  |  LP  |  pos  |  sel  |"<<std::endl;
    for (auto& e : this->elements){
        e.second->getState(state);
        std::cout << "    " << e.first; 
        std::cout << "    " << state.LP;
        std::cout << "    " << state.position;
        std::cout << "    " << state.selected;
        std::cout << "    " << std::endl;
    }
    */

    State state;
    
    std::vector<State> states;
    for (auto& e : this->elements){
        state.ID = e.first;
        e.second->getState(state);
        states.push_back(state);
    }
    (this->cplayer).update(states,this->spice,this->energy);
}

void Player::updateMovables(){
    State state;
    for (auto& e : this->elements){
        if (e.second->moves()) {

            int id = e.first;
            std::vector<Position>& path = e.second->get_remaining_path();  
            if (path.size() > 1) {
                e.second->getState(state);
                Position current_position(state.position);
                Position next_position = path.back();
                if (board.getCell(next_position.x, next_position.y).isOccupied()) {

                    while (board.getCell(next_position.x, next_position.y).isOccupied()) {
                        if (path.size() == 1) {
                            return;
                        }
                        path.pop_back();
                        next_position = path.back();
                    }
                    aStar aStar;
                    std::vector<Position> new_path = aStar.algorithm(current_position, next_position, board);
                    new_path.erase(new_path.begin());
                    while (new_path.size() > 1) {
                        path.push_back(new_path.front());
                        new_path.erase(new_path.begin());
                    }
                }
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

bool Player::place(Building& building,Position position){
    std::cout << "Placing a new building" << std::endl;
    return true;
}

bool Player::place(Refinery& building,Position& position){
    std::cout << "Placing a new refinery" << std::endl;
    return true;
}

bool Player::place(Unit& unit,std::vector<Position> positions){
    std::cout << "Placing a new unit" << std::endl;
    return true;
}
