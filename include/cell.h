#pragma once

#include <iostream>
#include <memory>

#include "common_utils.h"
#include "terrain.h"

class Cell {
 private:
    std::unique_ptr<Terrain> terrain;
    Position position;
    int occupied;

 public:
    Cell();
    void setPosition(int x, int y);
    void setTerrain(cell_t terrain);
    Position getPosition();
    void occupy(player_t faction);
    int  disoccupy();
    bool  isOccupied();
    bool canTraverse();
    bool canExtract();
    bool canBuild();
    void print();
};

