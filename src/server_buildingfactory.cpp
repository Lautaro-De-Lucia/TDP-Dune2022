#include "server_buildingfactory.h"

/*
https://stackoverflow.com/questions/54830073/how-make-the-factory-design-pattern-in-c
*/

std::unique_ptr<Building> BuildingFactory::manufacture(building_t unit,player_t faction, int ID) {
    switch (unit) {
        case AIR_TRAP:
            return std::unique_ptr<AirTrap>(new AirTrap(ID,faction,AIR_TRAP_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),AIR_TRAP_DIM_X,AIR_TRAP_DIM_Y,AIR_TRAP_CAPACITY));
            break;
        case BARRACK:
            return std::unique_ptr<Barrack>(new Barrack(ID,faction,BARRACK_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),BARRACK_DIM_X,BARRACK_DIM_Y));
            break;
        case LIGHT_FACTORY:
            return std::unique_ptr<LightFactory>(new LightFactory(ID,faction,LIGHT_FACTORY_LP,LIGHT_FACTORY_SPICE,LIGHT_FACTORY_ENERGY,Position(0,0),LIGHT_FACTORY_DIM_X,LIGHT_FACTORY_DIM_Y));
            break; 
        case HEAVY_FACTORY:
            return std::unique_ptr<HeavyFactory>(new HeavyFactory(ID,faction,HEAVY_FACTORY_LP,HEAVY_FACTORY_SPICE,HEAVY_FACTORY_ENERGY,Position(0,0),HEAVY_FACTORY_DIM_X,HEAVY_FACTORY_DIM_Y));
            break; 
        case SILO:
            return std::unique_ptr<Silo>(new Silo(ID,faction,SILO_LP,SILO_SPICE,SILO_ENERGY,Position(0,0),SILO_DIM_X,SILO_DIM_Y));
            break;    
        case PALACE:
            return std::unique_ptr<Palace>(new Palace(ID,faction,PALACE_LP,PALACE_SPICE,PALACE_ENERGY,Position(0,0),PALACE_DIM_X,PALACE_DIM_Y));
            break;
        case REFINERY:
            return std::unique_ptr<Refinery>(new Refinery(ID,faction,REFINERY_LP,AIR_TRAP_SPICE,AIR_TRAP_ENERGY,Position(0,0),REFINERY_DIM_X,REFINERY_DIM_Y,REFINERY_CAPACITY));
            break;
        default:
            std::cerr << "No such thing" << std::endl;
    }
    throw std::runtime_error("Unknown building"); 
}
