#pragma once
#include <string>
#include <iostream>
#include "board.h"

enum unit_t{
    HARVESTER = 1,
    TRIKE = 2
};

enum building_t {
    AIR_TRAP = 1, 
    BARRACK = 2,
    REFINERY = 3
};

struct State {
    uint16_t ID;
    uint16_t LP;
    Position position;
    bool selected;
};

struct Area {
    uint16_t Xmin;
    uint16_t Xmax;
    uint16_t Ymin;
    uint16_t Ymax;
    Area(uint16_t Xmin,uint16_t Xmax, uint16_t Ymin, uint16_t Ymax);
};