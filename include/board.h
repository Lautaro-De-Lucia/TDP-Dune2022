#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <cmath>

#include "cell.h"
#include "selectable.h"

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
    std::map<unit_t,int> creators;
    std::vector<Position> deposit_positions;
 public:
    Board(std::vector<std::vector<cell_t>> cell_types,std::map<int,std::unique_ptr<Selectable>> & server_elements);
    Cell& getCell(int x, int y);
    int getDimX(){return this-> dim_x;}
    int getDimY(){return this-> dim_y;}
    int getCreator(unit_t type);
    std::unique_ptr<Selectable>& getElementAt(int x, int y);
    void makeCreator(int ID);
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
    //  void move_unit(Position from, Position to,player_t faction,unit_t unit);
    std::vector<Position> get_traversable_neighbors_of(Position pos, size_t distance);
    size_t get_distance_between(Position pos1, Position pos2);
    void addDepositPositions(std::vector<Position> & new_deposit_positions);
    std::vector<Position> & getDepositPositions();
    std::vector<Position> getSurroundings(Position position, int e_dim_x, int e_dim_y);
    void removeCreator(unit_t unit);
};
