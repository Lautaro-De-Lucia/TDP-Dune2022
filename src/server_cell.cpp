#include "server_cell.h"

Cell::Cell(){
    this->position.x = 0;
    this->position.y = 0;
    this->element_ID = -1;
    this->reserved = -1;
}

void Cell::setPosition(int x, int y){
    this->position.x = x;
    this->position.y = y;
}

void Cell::setTerrain(cell_t terrain){
    switch(terrain){
        case ROCK:
            this->terrain = std::unique_ptr<Rock>(new Rock());
            break;
        case SAND_EMPTY:
            this->terrain = std::unique_ptr<Sand>(new Sand(SAND_SPICE_EMPTY));
            break;
        case SAND_LITTLE:
            this->terrain = std::unique_ptr<Sand>(new Sand(SAND_SPICE_LITTLE));
            break;
        case SAND_SOME:
            this->terrain = std::unique_ptr<Sand>(new Sand(SAND_SPICE_SOME));
            break;
        case SAND_FULL:
            this->terrain = std::unique_ptr<Sand>(new Sand(SAND_SPICE_FULL));
            break;                       
        case DUNE:
            this->terrain = std::unique_ptr<Dune>(new Dune());
            break;
        case PIT:
            this->terrain = std::unique_ptr<Pit>(new Pit());
            break;
        default:
            throw std::runtime_error("WTF");         
    }
}

Position Cell::getPosition(){
    return this->position;
}

int Cell::getID(){
    return this->element_ID;
}

int Cell::getSpice(){
    return this->terrain->getSpice();
}

void Cell::occupy(int ID){this->element_ID = ID;}
void Cell::disoccupy(){
    this->element_ID = -1;
    this->reserved = -1;    
}

bool Cell::isOccupied(){return this->element_ID >= 0;}
bool Cell::canTraverse(){return terrain->canTraverse();}
//  bool Cell::canTraverse(){return (terrain->canTraverse() && !(this->isOccupied()));}
bool Cell::canHarvest(){return (terrain->canExtract() && !(this->isOccupied()));}
bool Cell::canBuild(){return (terrain->canBuild() && !(this->isOccupied()));}
bool Cell::canSlowDown(){return terrain->canSlowDown();}
size_t Cell::extractSpice(){return this->terrain->extractSpice();}

void Cell::print(){}

void Cell::reserve(int ID) {
    this->reserved = ID;
}

void Cell::unReserve() {
    this->reserved = -1;
}

int Cell::getReserveID() {
    return this->reserved;
}
