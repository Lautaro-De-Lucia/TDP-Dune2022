#include "unitfactory.h"

std::unique_ptr<Unit> UnitFactory::create(unit_t unit) {
    switch (unit) {
        
        //case HARVESTER:
        //    return std::make_unique<Harvester>(HARVESTER_LP,HARVESTER_SPICE_COST,Position(0,0),HARVESTER_DIM_X,HARVESTER_DIM_Y,HARVESTER_SPEED,0,HARVESTER_SPICE_CAPACITY);
        //    break;
        
        case TRIKE:
            return std::make_unique<Trike>(TRIKE_LP,TRIKE_SPICE_COST,Position(0,0),TRIKE_DIM_X,TRIKE_DIM_Y,TRIKE_SPEED,TRIKE_ATTACK);
            //return std::make_unique<Trike>(TRIKE_LP,TRIKE_SPICE_COST,Position(0,0),TRIKE_DIM_X,TRIKE_DIM_Y,TRIKE_SPEED,TRIKE_ATTACK);
            break;
        default:
            std::cout << "No such thing" << std::endl;
    }
    return std::make_unique<Unit>(0,0,Position(0,0),0,0,0); // REFACTOR: Handle this kind of stuff with an exception
}
