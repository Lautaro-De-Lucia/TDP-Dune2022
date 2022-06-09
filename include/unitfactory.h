
#pragma once

#include <exception>
#include <memory>
#include "unit.h"
#include "common_utils.h"

class UnitFactory{
 public:
    static std::unique_ptr<Unit> create(unit_t unit);
};
