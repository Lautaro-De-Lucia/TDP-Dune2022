#pragma once

#include <iostream>
#include <memory>

#include "common_utils.h"
#include "terrain.h"

class Cell {
 private:
    std::unique_ptr<Terrain> terrain;
    Position position;
    int element_ID;

 public:
    Cell();
    void setPosition(int x, int y);
    void setTerrain(cell_t terrain);
    Position getPosition();
    void occupy(int ID);
    size_t extractSpice();
    void  disoccupy();
    bool isOccupied();
    bool canTraverse();
    bool canHarvest();
    bool canBuild();
    void print();
};

