#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <cmath>

#include "server_cell.h"
#include "server_selectable.h"

enum status_t {
    SUCCESS = 0,
    FAILED_TO_ADD = 1
};

class Board{
 private:
    int dim_x;
    int dim_y;
    std::vector<std::vector<Cell>> cells;
    std::map<int,std::unique_ptr<Selectable>> & elements;
    std::map<player_t,std::map<unit_t,int>> creatorID;
    std::map<player_t,std::map<unit_t,int>> creators;
    std::map<player_t,std::vector<Position>> deposit_positions;
    std::vector<Position> changed_sand_positions;
    std::vector<Position> destroyed_positions;
 public:
    Board(std::vector<std::vector<cell_t>> cell_types,std::map<int,std::unique_ptr<Selectable>> & server_elements);
    Cell& getCell(int x, int y);
    int getDimX(){return this-> dim_x;}
    int getDimY(){return this-> dim_y;}
    int getCreator(player_t faction,unit_t type);
    std::unique_ptr<Selectable>& getElementAt(int x, int y);
    void makeCreator(int ID);
    void addUnitCreator(player_t faction,building_t type);
    void removeUnitCreator(player_t faction, unit_t type);
    int getTotalCreators(player_t faction, unit_t type);
    void print();
    void print(std::vector<Position> path);
    status_t canPlace(const Position& location, int dim_x, int dim_y);
    bool canDeposit(int x, int y, player_t faction);
    bool canHarvest(int x, int y);
    bool canTraverse(int x, int y);
    bool hasEnemy(int x, int y, player_t player_faction);
    void dealDamage(int x, int y, int damage);
    size_t get_width();
    size_t get_height();
    std::vector<Position> getChangedSandPositions(){return this->changed_sand_positions;}
    void clearSandPositions();
    void addSandPosition(int x, int y);
    std::vector<Position> get_traversable_neighbors_of(Position pos, size_t distance);
    size_t get_distance_between(Position pos1, Position pos2);
    void addDepositPositions(player_t faction,std::vector<Position> & new_deposit_positions);
    std::vector<Position> getDepositPositions(player_t faction);
    std::vector<Position> getSurroundings(Position position, int e_dim_x, int e_dim_y);
    void removeCreator(player_t faction,unit_t unit);
};
