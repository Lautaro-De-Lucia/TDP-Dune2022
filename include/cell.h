#pragma once
#include <iostream>
#include "terrain.h"

enum status_cl{
    UNNOCCUPIED = 0,
    OCCUPIED = 1
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

class Cell {
 private:
    Terrain terrain;
    Position position;
    //  TODO: The isue of cell visibility for different players ¿Should that logic be left to the client?

 public:
    Cell();
    void setPosition(uint16_t x, uint16_t y);
    Position getPosition();
    bool isOccupied();
    void occupy();
    void free();
    void print();
};

