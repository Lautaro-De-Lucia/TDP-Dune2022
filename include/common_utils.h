#pragma once

#include <string>
#include <iostream>

enum unit_t{
    HARVESTER = 1,
    TRIKE = 2
}; 

enum cell_t{
    ROCK = 1,
    SAND = 2,
    DUNE = 3
};

enum building_t {
    AIR_TRAP = 1, 
    BARRACK = 2,
    REFINERY = 3
};

struct Position {
 public:
    uint16_t x;
    uint16_t y;

    Position();
    Position(uint16_t x, uint16_t y);
    Position(const Position& o);
    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
    bool operator==( const Position& o ) const { return o.x == x && o.y == y; };
    Position operator+( const Position& o ) { return Position( o.x + x, o.y + y ); };
    Position& operator=(const Position& o) { if (this == &o) return *this; this->x = o.x; this->y = o.y; return *this; };
    void print();
};

struct State {
    uint16_t ID;
    uint16_t LP;
    Position position;
    bool selected;
};

struct Area {
    uint16_t Xmin;
    uint16_t Xmax;
    uint16_t Ymin;
    uint16_t Ymax;
    Area(uint16_t Xmin,uint16_t Xmax, uint16_t Ymin, uint16_t Ymax);
};
