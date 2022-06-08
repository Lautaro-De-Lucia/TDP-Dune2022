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
    Board(int dim_x, int dim_y);
    Cell& getCell(int x, int y);
    void print();
    void print(std::vector<Position> path);
    status_t place(const Position & location, int dim_x, int dim_y, bool isUnit);
    size_t get_width();
    size_t get_height();
    void move_unit(Position from, Position to);
};

inline Board board(45,25);

