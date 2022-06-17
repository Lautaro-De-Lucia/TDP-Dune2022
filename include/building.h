#pragma once

#include "board.h"
#include "selectable.h"
#include "common_utils.h"

class Building : public Selectable {
 protected:    
    int spice;
    int energy;
 public:
    Building(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y);
    int getSpice();
    int getEnergy();
    virtual bool place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
    virtual void react(int x, int y, Board& board);
};
class AirTrap : public Building {
    int c_energy;
 public:
    AirTrap(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y, int c_energy);
    bool place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
    int getCEnergy();
};

class Barrack : public Building {
 public:
    Barrack(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y);
    bool place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
};

class Refinery : public Building {
    int c_spice;
 public:
    Refinery(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y, int c_spice);
    bool place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
    int getCSpice();
};
