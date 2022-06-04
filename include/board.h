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
    uint16_t dim_x;
    uint16_t dim_y;
    std::vector<std::vector<Cell>> cells;
 public:
    Board(uint16_t dim_x, uint16_t dim_y);
    Cell& getCell(uint16_t x, uint16_t y);
    void print();
    void print(std::vector<Position> path);
    status_t place(const Position & location, uint8_t dim_x, uint8_t dim_y);
    size_t get_width();
    size_t get_height();
};
