#include "buildingfactory.h"

/*
https://stackoverflow.com/questions/54830073/how-make-the-factory-design-pattern-in-c
*/

std::unique_ptr<Building> BuildingFactory::manufacture(building_t unit) {
    switch (unit) {
        case AIR_TRAP:
            return std::make_unique<AirTrap>(AIR_TRAP_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),AIR_TRAP_DIM_X,AIR_TRAP_DIM_Y,AIR_TRAP_CAPACITY);
            break;
        case BARRACK:
            return std::make_unique<Barrack>(BARRACK_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),BARRACK_DIM_X,BARRACK_DIM_Y);
            break;
        case REFINERY:
            return std::make_unique<Refinery>(REFINERY_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),REFINERY_DIM_X,REFINERY_DIM_Y,REFINERY_CAPACITY);
            break;
        default:
            std::cout << "No such thing" << std::endl;
    }
    return std::make_unique<Building>(0,0,0,Position(0,0),0,0); // REFACTOR: Handle this kind of stuff with an exception
}
