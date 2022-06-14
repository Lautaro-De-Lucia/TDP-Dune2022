#include "cell.h"

Cell::Cell(){
    this->position.x = 0;
    this->position.y = 0;
    this->faction = -1;
    this->building = -1;
    this->unit = -1;
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

void Cell::occupy(player_t faction,building_t building){
    this->faction = (int) faction;
    this->building = (int) building;
    this->unit = -1;
}

void Cell::occupy(player_t faction,unit_t unit){
    this->faction = (int) faction;
    this->unit = (int) unit;
    this->building = -1;
}


void Cell::disoccupy(){
    this->faction = -1;
    this->building = -1;
    this->unit = -1;
}

bool Cell::isOccupied(){return this->faction > 0;}
bool Cell::canTraverse(){return (terrain->canTraverse() && !(this->isOccupied()));}
bool Cell::canHarvest(){return (terrain->canExtract() && !(this->isOccupied()));}
bool Cell::canBuild(){return (terrain->canBuild() && !(this->isOccupied()));}
bool Cell::canDeposit(player_t faction){ return this->building == REFINERY && this->faction == faction;}
size_t Cell::extractSpice(){return this->terrain->extractSpice();}

void Cell::print(){}