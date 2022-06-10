#include "buildingfactory.h"

/*
https://stackoverflow.com/questions/54830073/how-make-the-factory-design-pattern-in-c
*/

std::unique_ptr<Building> BuildingFactory::manufacture(building_t unit,player_t faction) {


    switch (unit) {
        case AIR_TRAP:
            return std::unique_ptr<AirTrap>(new AirTrap(faction,AIR_TRAP_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),AIR_TRAP_DIM_X,AIR_TRAP_DIM_Y,AIR_TRAP_CAPACITY));
            break;
        case BARRACK:
            return std::unique_ptr<Barrack>(new Barrack(faction,BARRACK_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),BARRACK_DIM_X,BARRACK_DIM_Y));
            break;
        case REFINERY:
            return std::unique_ptr<Refinery>(new Refinery(faction,REFINERY_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),REFINERY_DIM_X,REFINERY_DIM_Y,REFINERY_CAPACITY));
            break;
        default:
            std::cerr << "No such thing" << std::endl;
    }
    throw std::runtime_error("Unknown building"); 
}
