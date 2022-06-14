#pragma once

#include <iostream>
#include <memory>

#include "common_utils.h"
#include "terrain.h"

class Cell {
 private:
    std::unique_ptr<Terrain> terrain;
    Position position;
    int faction;
    int building;
    int unit;

 public:
    Cell();
    void setPosition(int x, int y);
    void setTerrain(cell_t terrain);
    Position getPosition();
    void occupy(player_t faction, building_t building);
    void occupy(player_t faction, unit_t unit);
    size_t extractSpice();
    void  disoccupy();
    bool isOccupied();
    bool canTraverse();
    bool canHarvest();
    bool canBuild();
    bool canDeposit(player_t faction);
    void print();
};

