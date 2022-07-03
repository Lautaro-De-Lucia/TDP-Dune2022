#include "common_utils.h"

const char * stringify(player_t faction){
    switch(faction){
        case ATREIDES:
            return "atreides";
        case HARKONNEN:
            return "harkonnen";
        case ORDOS:
            return "ordos";
    }
    throw std::runtime_error("Unknown Faction");
}

const char * stringify(unit_t unit){
    switch(unit){
        case HARVESTER:
            return "harvester";
        case TRIKE:
            return "trike";
        case FREMEN:
            return "fremen";
        case INFANTRY:
            return "infantry";
        case SARDAUKAR:
            return "sardaukar";
        case TANK:
            return "tank";
        case DEVASTATOR:
            return "devastator";
    }
    throw std::runtime_error("Unknown Unit");
}

const char * stringify(building_t building){
    switch(building){
        case AIR_TRAP:
            return "windtrap";
        case BARRACK:
            return "barrack";
        case REFINERY:
            return "refinery";
        case LIGHT_FACTORY:
            return "lightfactory";
        case HEAVY_FACTORY:
            return "heavyfactory";
        case SILO:
            return "silo";
        case PALACE:
            return "palace";
        case CONSTRUCTION_YARD:
            return "yard";
    }
    throw std::runtime_error("Unknown Unit");
}

const char * stringify(direction_t dir){
    switch(dir){
        case TOP:
            return "top";
        case TOP_RIGHT:
            return "topright";
        case RIGHT:
            return "right";
        case BOTTOM_RIGHT:
            return "bottomright";
        case BOTTOM:
            return "bottom";
        case BOTTOM_LEFT:
            return "bottomleft";
        case LEFT:
            return "left";
        case TOP_LEFT:
            return "topleft";
    }
    throw std::runtime_error("Unknown Faction");
}

const char * stringify(frame_t animation){
    switch(animation){
        case ANIMATION_IDLE:
            return "idle";
        case MOVING_1:
            return "moving1";
        case MOVING_2:
            return "moving2";
        case SPECIAL:
            return "special";
    }
    throw std::runtime_error("Unknown Frame");
}

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
