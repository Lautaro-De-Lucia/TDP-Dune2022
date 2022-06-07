#pragma once

#include <string>
#include "board.h"
#include "common_utils.h"

class CSelectable {
private:
    std::string name;
    uint16_t LP;
    Position position;
    uint8_t dim_x;
    uint8_t dim_y;
    bool selected;
public:
    void update(State & new_state);
};