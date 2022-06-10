#include "player.h"

#include "astar.h"

#include <unistd.h>

bool game_has_not_started  = true;

std::vector<std::string> file_input;

Player::Player(player_t faction, int spice, int c_spice, int energy, int c_energy,Board & shared_board, CPlayer& client_player) 
: 
cplayer(client_player),
board(shared_board),
actions(0, std::vector<int>(0,0))
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
    
    //TextFileHandler command_reader(DATA_PATH INSTRUCTIONS_FILE);

    auto base_time_instruction = clock();

    while (1) {

        auto current_time = clock();
		auto frame_time_instruction = current_time - base_time_instruction;
       
        this->cplayer.updateCamera();
        reportState();

        if(frame_time_instruction > 100000 || game_has_not_started) {

            game_has_not_started = false;
            base_time_instruction = current_time;
            //std::cout << "game frame!!" << std::endl;
            command_t command;
            std::vector<int> action;
            if (actions.size() > 0) {
                action = actions[0];
                actions.pop_back();
                command = (command_t)action[0];
            } else command = IDLE;
            //if (command_reader.readInput(file_input)) {
                //std::system("clear");
                //  Read first line to get command
                //  With sockets: read 1 byte from the socket
                //command_t command = (command_t)(file_input[0][0]-'0');
                //std::cout << command << ": " << cmddict[command-1] <<std::endl;
                //printSeparator();
            switch (command){
                case CREATE_UNIT:
                    createUnit(action[1]);
                    break;
                case CREATE_BUILDING:
                    createBuilding(action[1], action[2], action[3]);
                    break;
                case MAKE_CREATOR:
                    makeCreator(action[1]);
                    break;
                case MOUSE_LEFT_CLICK:
                    handleLeftClick(action[1], action[2]);
                    break;
                case MOUSE_RIGHT_CLICK:
                    handleRightClick(action[1], action[2]);
                    break;
                case MOUSE_SELECTION:
                    handleSelection(action[1], action[2], action[3], action[4]);
                    break;
                case IDLE:
                    handleIdle();
                    sleep(1);
                    break;
                default:
                    break;
            } updateMovables();
        }
    }
}

void Player::addAction(std::vector<int> action){
    std::vector<int> v = action;
    actions.push_back(v);
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

void Player::createBuilding(int &type, int &pos_x, int &pos_y){

    //  Get Parameters
    //int type = (file_input[1][0]-'0');
    //int pos_x = std::stoi(file_input[2]);
    //int pos_y = std::stoi(file_input[3]);

    std::cout<< "Adding new building in position " << "("<< pos_x << "," << pos_y << ")" <<std::endl;
    //  Manufacture the building
    std::unique_ptr<Building> building = BuildingFactory::manufacture((building_t) type, this->faction);

    //  Attempt to add to board
    (this->place)((*building),Position(pos_x, pos_y));

    // Changing this->spice for 2000
    int test_spice = 2000;

    if ((*building).place(board,pos_x,pos_y,test_spice,this->c_spice,this->energy,this->c_energy)){
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
void Player::createUnit(int &type){
    //  Get Parameters
    //int type = (file_input[1][0]-'0');
    std::cout<< "Creating new unit" << std::endl;
    //  Attempt to add to board
    //  Create the unit
    std::vector<Position> positions = elements.at(creators.at((unit_t) type))->getSurroundings(); //  FAILING HERE
    
    std::unique_ptr<Unit> unit = UnitFactory::create((unit_t) type,this->faction);
    //  Attempt adding it
    //  REFACTOR: place should probably be a method of the Player class
        // place(Board & board,Building & building,Position & position)
        // place(Board & board,Unit & unit,std::vector<Positions> positions)

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
void Player::handleLeftClick(int &pos_x, int &pos_y){
    std::cout << "Client just did a left click on the map" << std::endl;
    //  Get positions
    //int pos_x = std::stoi(file_input[1]);
    //int pos_y = std::stoi(file_input[2]);
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
void Player::handleSelection(const int Xmin, const int Xmax, const int Ymin, const int Ymax){
    std::cout << "Client just selected a part of the map" << std::endl;
    //  Get selection limits
    //int Xmin = std::stoi(file_input[1]);
    //int Xmax = std::stoi(file_input[2]);
    //int Ymin = std::stoi(file_input[3]);
    //int Ymax = std::stoi(file_input[4]);
    std::cout << "Selection: (" << Xmin << "," << Xmax << "," << Ymin << "," << Ymax << ")" << std::endl;
    //  Traverse and mark as selected those that are included
    Area selection(Xmin,Xmax,Ymin,Ymax);
    for (auto & e : this->elements){
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
void Player::handleRightClick(int &pos_x, int &pos_y){
    std::cout << "Client just did a right click on the map" << std::endl;
    //  Get positions
    //int pos_x = std::stoi(file_input[1]);
    //int pos_y = std::stoi(file_input[2]);
    std::cout << "On position: " << Position(pos_x, pos_y) << std::endl;
    //  Traverse elements and make each selected unit handle the cell
    for (auto & e : this->elements){
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
    for (auto & e : this->elements){
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

    (this->cplayer).update(states);
}

void Player::updateMovables(){
    State state;
    for (auto& e : this->elements){
        if (e.second->moves()) {

            int id = e.first;
            std::vector<Position>& path = e.second->get_remaining_path();  
            if (path.size() != 0) {
                e.second->getState(state);
                Position current_position(state.position);
                Position next_position = path.back();
                path.pop_back();
                e.second->setPosition(next_position);
                board.move_unit(current_position, next_position, e.second->getFaction());
            }
        }
    }
}

void Player::makeCreator(int &building_ID){
    //int building_ID = (int) std::stoi(file_input[1]);
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
