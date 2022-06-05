#include "cell.h"

Cell::Cell(){
    this->position.x = 0;
    this->position.y = 0;
}

void Cell::setPosition(uint16_t x, uint16_t y){
    this->position.x = x;
    this->position.y = y;
}

Position Cell::getPosition(){
    return this->position;
}

bool Cell::isOccupied(){
    return this->terrain.isOccupied();
}
void Cell::occupy(){
    this->terrain.occupy();
}
void Cell::free(){
    this->terrain.free();
}

void Cell::print(){
    if(this->isOccupied())
        std::cout << "O ";
    else
        std::cout << ". ";
}