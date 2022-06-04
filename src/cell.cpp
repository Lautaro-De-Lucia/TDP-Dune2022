#include "cell.h"

Position::Position() {}

Position::Position(uint16_t x, uint16_t y) {
    this->x = x;
    this->y = y;
}

Position::Position(const Position& o) {
    this->x = o.x;
    this->y = o.y;
}

void Position::print() {
    std::cout << '(' << x << ',' << y << ')';
}

std::ostream& operator<<(std::ostream& os, const Position& pos)
{
    std::cout << '(' << pos.x << ',' << pos.y << ')';
    return os;
}

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