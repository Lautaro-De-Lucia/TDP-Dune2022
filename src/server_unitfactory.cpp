#include "server_unitfactory.h"

std::unique_ptr<Unit> UnitFactory::create(unit_t unit, player_t faction, int ID) {
    switch (unit) {
        case HARVESTER:
            return std::unique_ptr<Harvester>(new Harvester(ID,faction,HARVESTER_LP,HARVESTER_SPICE_COST,Position(0,0),HARVESTER_DIM_X,HARVESTER_DIM_Y,HARVESTER_SPEED,HARVESTER_SPICE_CAPACITY));
            break;
        case TRIKE:
            return std::unique_ptr<Trike>(new Trike(ID,faction,TRIKE_LP,TRIKE_SPICE_COST,Position(0,0),TRIKE_DIM_X,TRIKE_DIM_Y,TRIKE_SPEED,TRIKE_ATTACK,TRIKE_RANGE));
            break;
        default:
            std::cout << "No such thing" << std::endl;
    }
    throw std::runtime_error("Unknown unit"); 
}
