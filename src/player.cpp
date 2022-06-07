#include "player.h"

#include "astar.h"

#include <unistd.h>

size_t game_time = 0;

std::vector<std::string> file_input;

Player::Player(uint16_t spice, uint16_t c_spice, uint16_t energy, uint16_t c_energy) {
    this->ID = 0;
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

void Player::run(){

    TextFileHandler command_reader(DATA_PATH INSTRUCTIONS_FILE);

    while (command_reader.readInput(file_input)){
        std::system("clear");

        //  Read first line to get command
            //  With sockets: read 1 byte from the socket
        command_t command = (command_t)(file_input[0][0]-'0');
        std::cout << command << ": " << cmddict[command-1] <<std::endl;
        printSeparator();

        switch(command){
            case CREATE_UNIT:
                createUnit();
                board.print();
                this->print();
                break;
            case CREATE_BUILDING:
                createBuilding();
                board.print();
                this->print();
                break;
            case MAKE_CREATOR:
                makeCreator();
                board.print();
                this->print();    
                break;
            case MOUSE_LEFT_CLICK:
                handleLeftClick();
                board.print();
                this->print();
                break;
            case MOUSE_RIGHT_CLICK:
                handleRightClick();
                board.print();
                this->print();
                break;
            case MOUSE_SELECTION:
                handleSelection();
                board.print();
                this->print();
                break;
            case IDLE:
                handleIdle();
                board.print();
                this->print();
                break;
            default:
                break;
        }
        updateState();
        reportState();
        printSeparator();
        //  Render to screen
        usleep(500000);
        game_time++;
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
void Player::createBuilding(){

    //  Get Parameters
    uint8_t type = (uint8_t) (file_input[1][0]-'0');
    uint16_t pos_x = (uint16_t) std::stoi(file_input[2]);
    uint16_t pos_y = (uint16_t) std::stoi(file_input[3]);
    std::cout<< "Adding new building in position " << "("<< pos_x << "," << pos_y << ")" <<std::endl;
    //  Attempt to add to board
        //  Manufacture the building
    std::unique_ptr<Building> building = BuildingFactory::manufacture((building_t)type);
    //  Attempt adding it
    (this->place)((*building),Position(pos_x,pos_y));
    // changing this->spice for 2000
    uint16_t test_spice = 2000;
    if ((*building).place(board,pos_x,pos_y,test_spice,this->c_spice,this->energy,this->c_energy)){
        (this->elements).insert({ID++, std::move(building)});
        std::cout << "Building succesfully created" << std::endl;
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
void Player::handleLeftClick(){
    std::cout << "Client just did a left click on the map" << std::endl;
    //  Get positions
    uint16_t pos_x = (uint16_t) std::stoi(file_input[1]);
    uint16_t pos_y = (uint16_t) std::stoi(file_input[2]);
    std::cout << "On position: " << Position(pos_x,pos_y) << std::endl;
    //  Leave selected only the units at that position
    for (auto & e : this->elements){
        if(e.second->contains(Position(pos_x,pos_y)))
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
void Player::handleSelection(){
    std::cout << "Client just selected a part of the map" << std::endl;
    //  Get selection limits
    uint16_t Xmin = (uint16_t) std::stoi(file_input[1]);
    uint16_t Xmax = (uint16_t) std::stoi(file_input[2]);
    uint16_t Ymin = (uint16_t) std::stoi(file_input[3]);
    uint16_t Ymax = (uint16_t) std::stoi(file_input[4]);
    std::cout << "Selection: (" << Xmin << "," << Xmax << "," << Ymin << "," << Ymax << ")" << std::endl;
    //  Traverse and mark as selected those that are included
    Area selection(Xmin,Xmax,Ymin,Ymax);
    for (auto & e : this->elements){
        e.second->unselect();
        if(e.second->isWithin(selection))
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
void Player::handleRightClick(){
    std::cout << "Client just did a right click on the map" << std::endl;
    //  Get positions
    uint16_t pos_x = (uint16_t) std::stoi(file_input[1]);
    uint16_t pos_y = (uint16_t) std::stoi(file_input[2]);
    std::cout << "On position: " << Position(pos_x,pos_y) << std::endl;
    //  Traverse elements and make each selected unit handle the cell
    for (auto & e : this->elements){
        if(e.second->isSelected()){
            std::cout << e.first << ": ";
            e.second->react(board.getCell(pos_x,pos_y));
        }
    }
}
/*
We only need unit <type>
[TYPE](8)
*/

void Player::handleIdle(){
    std::cout << "Client just did nothing" << std::endl;
}

void Player::createUnit(){
    //  Get Parameters
    uint8_t type = (uint8_t) (file_input[1][0]-'0');
    std::cout<< "Creating new unit" << std::endl;
    //  Attempt to add to board
        //  create the unit
    std::vector<Position> positions = elements.at(creators.at((unit_t)type))->getSurroundings(); //  FAILING HERE
    std::unique_ptr<Trike> unit = TrikeFactory::create((unit_t)type);
    //  Attempt adding it
    //  REFACTOR: place should probably be a method of the Player class
        // place(Board & board,Building & building,Position & position)
        // place(Board & board,Unit & unit,std::vector<Positions> positions)

    if ((*unit).place(board,positions,this->spice)){
        (this->elements).insert({ID++, std::move(unit)});
        
        std::cout << "Unit succesfully created" << std::endl;
        return;
    }
    std::cout << "Can't build here" << std::endl;
}
/*
(RECEIVE)
    CASE SUCCESS: 
        SERVER ------------>{...,[sel][pos_y][pos_x][LP][ID],...}[total]---------> CLIENT
*/
void Player::reportState(){
    State state;
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
}

void Player::updateState(){
    State state;
    for (auto & e : this->elements){
        if (e.second->moves()) {

            uint16_t id = e.first;

            std::vector<Position>& path = e.second->get_remaining_path();  
            if (path.size() != 0) {

                std::cout << "path_size() != 0: ID number " << id << " moves..." << '\n';

                e.second->getState(state);
                Position current_position(state.position);
                Position next_position = path.back();
                path.pop_back();
                e.second->setPosition(next_position);
                board.move_unit(current_position, next_position);
            }
        }
    }
}


void Player::makeCreator(){
    uint16_t building_ID = (uint16_t) std::stoi(file_input[1]);
    if (this->elements.at(building_ID)->getName() == "Refinery")
        this->creators[HARVESTER] = building_ID; 
    if (this->elements.at(building_ID)->getName() == "Barrack"){
        this->creators[TRIKE] = building_ID;     
    }
    std::cout << this->elements.at(building_ID)->getName() << " of ID: " << building_ID << " is now a creator" << std::endl;
}

bool Player::place(Building & building,Position position){
    std::cout << "Placing a new building" << std::endl;
    return true;
}
bool Player::place(Refinery & building,Position & position){
    std::cout << "Placing a new refinery" << std::endl;
    return true;
}
bool Player::place(Trike & unit,std::vector<Position> positions){
    std::cout << "Placing a new unit" << std::endl;
    return true;
}
