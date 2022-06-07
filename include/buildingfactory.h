#pragma once

#include <memory>
#include "building.h"
#include "common_utils.h"

class BuildingFactory{
 private:
 public:
    static std::unique_ptr<Building> manufacture(building_t unit);
};