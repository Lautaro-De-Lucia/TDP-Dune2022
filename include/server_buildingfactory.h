#pragma once

#include <exception>
#include <memory>

#include "server_building.h"

#include "common_utils.h"

class BuildingFactory{
 public:
    static std::unique_ptr<Building> manufacture(building_t unit, player_t faction,int ID);
};
