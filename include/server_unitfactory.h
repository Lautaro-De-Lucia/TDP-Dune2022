
#pragma once

#include <exception>
#include <memory>

#include "server_unit.h"

#include "common_utils.h"

class UnitFactory{
 public:
    static std::unique_ptr<Unit> create(unit_t unit, player_t faction,int ID);
};
