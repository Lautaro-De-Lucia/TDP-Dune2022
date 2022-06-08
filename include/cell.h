#pragma once

#include <iostream>

#include "terrain.h"
#include "common_utils.h"

enum status_cl{
    UNNOCCUPIED = 0,
    OCCUPIED = 1
};

class Cell {
 private:
    Terrain terrain;
    Position position;
    bool unit;
    //  TODO: The isue of cell visibility for different players ¿Should that logic be left to the client?

 public:
    Cell();
    void setPosition(int x, int y);
    Position getPosition();
    bool isOccupied();
    bool hasUnit();
    void placeUnit();
    void removeUnit();
    void occupy();
    void free();
    void print();
};

