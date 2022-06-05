#include "common_utils.h"

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