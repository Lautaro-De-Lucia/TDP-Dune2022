#pragma once
#include "selectable.h"

enum building_t {
    AIR_TRAP = 1, 
    BARRACK = 2,
    REFINERY = 3
};

class Building : public Selectable {
protected:    
    uint16_t spice;
    uint16_t energy;
public:
    Building(uint16_t LP,uint16_t spice,uint16_t energy,Position pos,uint8_t dim_x, uint8_t dim_y);
    uint16_t getSpice();
    uint16_t getEnergy();
    virtual bool place(Board & board,uint16_t pos_x, uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity);
};
class AirTrap : public Building {
    uint16_t c_energy;
public:
    AirTrap(uint16_t LP,uint16_t spice,uint16_t energy,Position pos, uint8_t dim_x, uint8_t dim_y, uint16_t c_energy);
    bool place(Board & board,uint16_t pos_x,uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity);
};

class Barrack : public Building {
public:
    Barrack(uint16_t LP,uint16_t spice,uint16_t energy, Position pos,uint8_t dim_x, uint8_t dim_y);
    bool place(Board & board,uint16_t pos_x,uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity);
};

class Refinery : public Building {
    uint16_t c_spice;
public:
    Refinery(uint16_t LP,uint16_t spice,uint16_t energy, Position pos, uint8_t dim_x, uint8_t dim_y, uint16_t c_spice);
    bool place(Board & board,uint16_t pos_x,uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity);
};