#include "building.h"

Building::Building(int LP,int spice,int energy, Position pos, int dim_x, int dim_y)
:
Selectable(LP,pos,dim_x,dim_y,false)
{
    this->spice = spice;
    this->energy = energy;
}

bool Building::place(Board & board,int pos_x, int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity){
    if ((spice - this->getSpice()) < 0){
        std::cerr << "Not enough Spice!!!" << std::endl;
        return false;
    }
    if ((energy - this->getEnergy()) < 0){
        std::cerr << "Not enough Energy!!!" << std::endl;
        return false;
    }
    if (board.place(Position(pos_x,pos_y),this->getDimX(),this->getDimY(), false) == FAILED_TO_ADD){
        std::cerr << "Can't build in this position!!!" << std::endl;
        return false;
    }
    this->setPosition(Position(pos_x,pos_y));
    return true;
}

int Building::getSpice(){
    return this->spice;
}
int Building::getEnergy(){
    return this->energy;
}

AirTrap::AirTrap(int LP,int spice,int energy, Position pos, int dim_x,int dim_y, int c_energy)
:
Building(LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Wind Trap";
    this->c_energy = c_energy;
}

bool AirTrap::place(Board & board,int pos_x,int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    spice -= this->spice;
    energy += this->energy;
    return true;
}

Barrack::Barrack(int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Barrack";
}

bool Barrack::place(Board & board,int pos_x,int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    spice -= this->spice;
    energy -= this->energy;
    return true;
}



Refinery::Refinery(int LP,int spice,int energy, Position pos, int dim_x,int dim_y, int c_spice)
:
Building(LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Refinery";
    this->c_spice= c_spice;
}

bool Refinery::place(Board & board,int pos_x,int pos_y,int & spice,int & spice_capacity,int & energy,int & energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    spice -= this->spice;
    energy -= this->energy;
    spice_capacity += this->c_spice; 
    return true;
}

