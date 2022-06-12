#include "common_utils.h"

std::map<color_t,SDL_Color> colors = {
    {RED,SDL_Color{250,0,0,0}},
    {GREEN,SDL_Color{0,250,0,0}},
    {BLUE,SDL_Color{0,0,250,0}}
};

Position::Position() {}

Position::Position(int x, int y) {
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

void sleepcp(int milliseconds) // Cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}
