#pragma once

#include <iostream>
#include <memory>

#include "server_terrain.h"

#include "common_utils.h"

class Cell {
 private:
    std::unique_ptr<Terrain> terrain;
    Position position;
    int element_ID;
    int reserved;

 public:
    Cell();
    void setPosition(int x, int y);
    void setTerrain(cell_t terrain);
    Position getPosition();
    int getID();
    int getSpice();
    void occupy(int ID);
    size_t extractSpice();
    void  disoccupy();
    bool isOccupied();
    bool canTraverse();
    bool canHarvest();
    bool canBuild();
    void print();
    void reserve(int ID);
    void unReserve();
    int getReserveID();
};

