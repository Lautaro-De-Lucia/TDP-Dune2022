#pragma once

#include "server_board.h"
#include "server_selectable.h"

#include "common_utils.h"

class Building : public Selectable {
 protected:    
    int spice;
    int energy;
 public:
    Building(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y);
    int getSpice();
    int getEnergy();
    virtual response_t place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
    virtual void react(int x, int y, Board& board);
    virtual void sendState(Protocol & protocol,Socket & client_socket);
};
class AirTrap : public Building {
    int c_energy;
 public:
    AirTrap(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y, int c_energy);
    response_t place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
    int getCEnergy();
    virtual void sendState(Protocol & protocol,Socket & client_socket);
};

class Barrack : public Building {
 public:
    Barrack(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y);
    response_t place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
    virtual void sendState(Protocol & protocol,Socket & client_socket);
};

class Refinery : public Building {
    int c_spice;
 public:
    Refinery(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y, int c_spice);
    response_t place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity);
    int getCSpice();
    virtual void sendState(Protocol & protocol,Socket & client_socket);
};