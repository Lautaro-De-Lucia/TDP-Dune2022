#pragma once

#include "selectable.h"
#include "common_utils.h"

class Building : public Selectable {
protected:    
    int spice;
    int energy;
public:
    Building(int LP,int spice,int energy,Position pos,int dim_x, int dim_y);
    int getSpice();
    int getEnergy();
    virtual bool place(Board & board,int pos_x, int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity);
};
class AirTrap : public Building {
    int c_energy;
public:
    AirTrap(int LP,int spice,int energy,Position pos, int dim_x, int dim_y, int c_energy);
    bool place(Board & board,int pos_x,int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity);
};

class Barrack : public Building {
public:
    Barrack(int LP,int spice,int energy, Position pos,int dim_x, int dim_y);
    bool place(Board & board,int pos_x,int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity);
};

class Refinery : public Building {
    int c_spice;
public:
    Refinery(int LP,int spice,int energy, Position pos, int dim_x, int dim_y, int c_spice);
    bool place(Board & board,int pos_x,int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity);
};