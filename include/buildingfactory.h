#pragma once

#include <memory>
#include "building.h"

//  These would be lifted from a file
#define AIR_TRAP_LP 500
#define AIR_TRAP_ENERGY 500
#define AIR_TRAP_SPICE 800
#define AIR_TRAP_DIM_X 3
#define AIR_TRAP_DIM_Y 3
#define AIR_TRAP_CAPACITY 500

#define BARRACK_LP 300
#define BARRACK_ENERGY 100
#define BARRACK_SPICE 300
#define BARRACK_DIM_X 2
#define BARRACK_DIM_Y 3

#define REFINERY_LP 1000
#define REFINERY_ENERGY 400
#define REFINERY_SPICE 500
#define REFINERY_DIM_X 3
#define REFINERY_DIM_Y 3
#define REFINERY_CAPACITY 5000

class BuildingFactory{
 private:
 public:
    static std::unique_ptr<Building> manufacture(building_t unit);
};