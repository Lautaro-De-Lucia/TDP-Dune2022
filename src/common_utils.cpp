#include "common_utils.h"

std::map<color_t,SDL_Color> colors = {
    {RED,SDL_Color{250,0,0,0}},
    {GREEN,SDL_Color{0,250,0,0}},
    {BLUE,SDL_Color{0,0,250,0}},
    {YELLOW,SDL_Color{250,250,0,0}}
};

Position::Position() {
    this->x = -1;
    this->y = -1;
}

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

// revisar
Position pixelToGP(int x, int y) {
    Position pos;
    int i = 0;
    while (true){
        if(x < i*TILE_SIZE*2){
            pos.x = i-1;
            break;
        }
        i++;
    }
    i = 0;
    while (true){
        if(y < i*TILE_SIZE*2){
            pos.y = i-1;
            break;
        }
        i++;
    }

    return pos;
}

// revisar
void GPToPixel(Position pos, int& pixel_x, int& pixel_y) {

    pixel_x = 2 * pos.x * TILE_SIZE + TILE_SIZE;
    pixel_y = 2 * pos.y * TILE_SIZE + TILE_SIZE;
}
