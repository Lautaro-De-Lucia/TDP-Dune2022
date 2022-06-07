#pragma once

#include <memory>
#include "unit.h"
#include "building.h"

//  These would be lifted from a file
#define HARVESTER_LP 500
#define HARVESTER_SPICE_COST 500
#define HARVESTER_SPICE_CAPACITY 800
#define HARVESTER_SPEED 2
#define HARVESTER_DIM_X 1
#define HARVESTER_DIM_Y 1

#define TRIKE_LP 800
#define TRIKE_SPICE_COST 500
#define TRIKE_SPEED 5
#define TRIKE_ATTACK 3
#define TRIKE_DIM_X 1
#define TRIKE_DIM_Y 1

class TrikeFactory{
 public:
    static std::unique_ptr<Trike> create(unit_t unit);
};
