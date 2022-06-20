#include "server_unitfactory.h"

std::unique_ptr<Unit> UnitFactory::create(unit_t unit, player_t faction, int ID) {
    switch (unit) {
        case HARVESTER:
            return std::unique_ptr<Harvester>(new Harvester(ID,faction,HARVESTER_LP,HARVESTER_SPICE_COST,Position(0,0),1,1,HARVESTER_SPEED,HARVESTER_SPICE_CAPACITY));
            break;
        case TRIKE:
            return std::unique_ptr<Trike>(new Trike(ID,faction,TRIKE_LP,TRIKE_SPICE_COST,Position(0,0),1,1,TRIKE_SPEED,TRIKE_ATTACK,TRIKE_RANGE));
            break;
        case FREMEN:
            return std::unique_ptr<Fremen>(new Fremen(ID,faction,FREMEN_LP,FREMEN_SPICE_COST,Position(0,0),1,1,FREMEN_SPEED,FREMEN_ATTACK,FREMEN_RANGE));
            break;
        case INFANTRY:
            return std::unique_ptr<Infantry>(new Infantry(ID,faction,INFANTRY_LP,INFANTRY_SPICE_COST,Position(0,0),1,1,INFANTRY_SPEED,INFANTRY_ATTACK,INFANTRY_RANGE));
            break;
        case SARDAUKAR:
            return std::unique_ptr<Sardaukar>(new Sardaukar(ID,faction,SARDAUKAR_LP,SARDAUKAR_SPICE_COST,Position(0,0),1,1,SARDAUKAR_SPEED,SARDAUKAR_ATTACK,SARDAUKAR_RANGE));
            break;
        case TANK:
            return std::unique_ptr<Tank>(new Tank(ID,faction,TANK_LP,TANK_SPICE_COST,Position(0,0),1,1,TANK_SPEED,TANK_ATTACK,TANK_RANGE));
            break;
        case DEVASTATOR:
            return std::unique_ptr<Devastator>(new Devastator(ID,faction,DEVASTATOR_LP,DEVASTATOR_SPICE_COST,Position(0,0),1,1,DEVASTATOR_SPEED,DEVASTATOR_ATTACK,DEVASTATOR_RANGE));
            break;
        default:
            std::cout << "No such thing" << std::endl;
    }
    throw std::runtime_error("Unknown unit"); 
}
