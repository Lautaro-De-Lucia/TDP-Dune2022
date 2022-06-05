#pragma once

#include <iostream>
#include <vector>
#include "common_utils.h"

class MapCell{

};

class GameMap {
    size_t dim_x;
    size_t dim_y;
    std::vector<std::vector<MapCell>> cells;
};
