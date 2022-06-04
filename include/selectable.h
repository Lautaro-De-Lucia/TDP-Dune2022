#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "board.h"

struct SCDesc {
    std::string name;
    uint16_t LP;
    uint8_t dim_x;
    uint8_t dim_y;
    uint16_t Ymax;
};

struct Area {
    uint16_t Xmin;
    uint16_t Xmax;
    uint16_t Ymin;
    uint16_t Ymax;
    Area(uint16_t Xmin,uint16_t Xmax, uint16_t Ymin, uint16_t Ymax);
};

struct State{
    uint16_t LP;
    Position position;
    bool selected;
};

class Selectable {
    protected:
    std::string name;
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
    SCDesc description();
    std::vector<Position> getPositions();
    std::vector<Position> getSurroundings(); 
    void setPosition(Position position);
    Position getPosition();
    uint8_t getDimX();
    uint8_t getDimY();
    void getState(State & state);
};

