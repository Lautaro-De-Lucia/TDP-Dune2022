#include "cell.h"

Cell::Cell(){
    this->position.x = 0;
    this->position.y = 0;
    this->unit = false;
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

void Cell::placeUnit() {
    this->unit = true;
    this->terrain.occupy();
}

void Cell::removeUnit() {
    this->unit = false;
    this->terrain.free();
}

bool Cell::hasUnit(){
    return this->unit;
}

void Cell::occupy(){
    this->terrain.occupy();
}
void Cell::free(){
    this->terrain.free();
}

void Cell::print(){
    if (this->hasUnit())
        std::cout << "X ";
    else if (this->isOccupied())
        std::cout << "O ";
    else
        std::cout << ". ";
}