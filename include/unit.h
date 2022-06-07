#pragma once 

#include "selectable.h"
#include "common_utils.h"

#include "astar.h"

/*
class Unit : public Selectable {
 protected:
    std::string name;
    uint16_t spice;
    uint16_t speed;

 public:
    Unit() = default;
    Unit(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed);
    bool place(Board & board,std::vector<Position> & positions,uint16_t & spice);
    virtual void react(Cell & location);
    virtual ~Unit();
};
*/


/*
class Harvester : public Unit {
   uint16_t stored_spice;
   uint16_t max_spice;
public:
    Harvester(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed,uint16_t stored_spice, uint16_t max_spice);
    void create();
    void react(Cell & location);
};
*/
class Trike : public Selectable {
   std::string name;
   uint16_t spice;
   uint16_t speed;
   uint16_t attack;
public:
    Trike(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed,uint16_t attack);
    void create();
    void react(Cell & location);
    bool place(Board & board,std::vector<Position> & positions,uint16_t & spice);
    ~Trike();
};
