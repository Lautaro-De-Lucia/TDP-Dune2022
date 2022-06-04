#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "common_utils.h"

class Selectable {
    protected:
    std::string name;
    std::string imagepath;
    uint16_t LP;
    Position position;
    uint8_t dim_x;
    uint8_t dim_y;
    bool selected;

    public:
    Selectable(uint16_t LP,Position pos, uint8_t dim_x, uint8_t dim_y);
    void selection(Area & mouse_area);
    bool isSelected();
    void select();
    void unselect();
    bool contains(const Position & position);
    bool isWithin(const Area & selection);
    void react(Cell & location);
    std::string getName();
    std::vector<Position> getPositions();
    std::vector<Position> getSurroundings(); 
    void setPosition(Position position);
    Position getPosition();
    uint8_t getDimX();
    uint8_t getDimY();
    void getState(State & state);
};

