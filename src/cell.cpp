#include "cell.h"

Cell::Cell(){
    this->position.x = 0;
    this->position.y = 0;
    this->occupied = 0;
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

void Cell::occupy(player_t faction){
    this->occupied = (int) faction;
}
int Cell::disoccupy(){
    int current = this->occupied;
    this->occupied = 0;
    return current;
}

bool Cell::isOccupied(){return (this-> occupied > 0 ? true : false);}
bool Cell::canTraverse(){return (terrain->canTraverse() && !(this->isOccupied()))? true : false;}
bool Cell::canExtract(){return (terrain->canExtract() && !(this->isOccupied()))? true : false;}
bool Cell::canBuild(){return (terrain->canBuild() && !(this->isOccupied()))? true : false;}

void Cell::print(){}