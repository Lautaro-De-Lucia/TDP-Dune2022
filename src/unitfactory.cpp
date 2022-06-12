#include "unitfactory.h"

std::unique_ptr<Unit> UnitFactory::create(unit_t unit, player_t faction) {
    switch (unit) {
        case HARVESTER:
            return std::unique_ptr<Harvester>(new Harvester(faction,HARVESTER_LP,HARVESTER_SPICE_COST,Position(0,0),HARVESTER_DIM_X,HARVESTER_DIM_Y,HARVESTER_SPEED,0,HARVESTER_SPICE_CAPACITY));
            break;
        case TRIKE:
            return std::unique_ptr<Trike>(new Trike(faction,TRIKE_LP,TRIKE_SPICE_COST,Position(0,0),TRIKE_DIM_X,TRIKE_DIM_Y,TRIKE_SPEED,TRIKE_ATTACK));
            break;
        default:
            std::cout << "No such thing" << std::endl;
    }
    throw std::runtime_error("Unknown unit"); 
}
