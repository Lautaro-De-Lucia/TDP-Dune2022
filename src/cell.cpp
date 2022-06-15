#include "cell.h"

Cell::Cell(){
    this->position.x = 0;
    this->position.y = 0;
    this->element_ID = -1;
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
        case SAND:
            this->terrain = std::unique_ptr<Sand>(new Sand());
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

void Cell::occupy(int ID){this->element_ID = ID;}
void Cell::disoccupy(){this->element_ID = -1;}

bool Cell::isOccupied(){return this->element_ID >= 0;}
bool Cell::canTraverse(){return (terrain->canTraverse() && !(this->isOccupied()));}
bool Cell::canHarvest(){return (terrain->canExtract() && !(this->isOccupied()));}
bool Cell::canBuild(){return (terrain->canBuild() && !(this->isOccupied()));}
size_t Cell::extractSpice(){return this->terrain->extractSpice();}

void Cell::print(){}