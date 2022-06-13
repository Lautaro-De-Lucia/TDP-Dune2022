#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "cell.h"

enum status_t {
    SUCCESS = 0,
    FAILED_TO_ADD = 1
};



class Board{
 private:
    int dim_x;
    int dim_y;
    std::vector<std::vector<Cell>> cells;
 public:
    Board(std::vector<std::vector<cell_t>> cell_types);
    Cell& getCell(int x, int y);
    void print();
    void print(std::vector<Position> path);
    status_t place(const Position& location, int dim_x, int dim_y, player_t faction);
    size_t get_width();
    size_t get_height();
    void move_unit(Position from, Position to,player_t faction);
    std::vector<Position> get_unoccupied_neighbors_of(Position pos);
};