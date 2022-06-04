#include "building.h"

Building::Building(uint16_t LP,uint16_t spice,uint16_t energy, Position pos, uint8_t dim_x, uint8_t dim_y)
:
Selectable(LP,pos,dim_x,dim_y)
{
    this->spice = spice;
    this->energy = energy;
}

bool Building::place(Board & board,uint16_t pos_x, uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity){
    if ((spice - this->getSpice()) < 0){
        std::cout << "Not enough Spice!!!" << std::endl;
        return false;
    }
    if ((energy - this->getEnergy()) < 0){
        std::cout << "Not enough Energy!!!" << std::endl;
        return false;
    }
    if (board.place(Position(pos_x,pos_y),this->getDimX(),this->getDimY()) == FAILED_TO_ADD){
        std::cout << "Can't build in this position!!!" << std::endl;
        return false;
    }
    this->setPosition(Position(pos_x,pos_y));
    return true;
}

uint16_t Building::getSpice(){
    return this->spice;
}
uint16_t Building::getEnergy(){
    return this->energy;
}

AirTrap::AirTrap(uint16_t LP,uint16_t spice,uint16_t energy, Position pos, uint8_t dim_x,uint8_t dim_y, uint16_t c_energy)
:
Building(LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Wind Trap";
    this->c_energy = c_energy;
}

bool AirTrap::place(Board & board,uint16_t pos_x,uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    spice -= this->spice;
    energy += this->energy;
    return true;
}

Barrack::Barrack(uint16_t LP,uint16_t spice,uint16_t energy, Position pos, uint8_t dim_x,uint8_t dim_y)
:
Building(LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Barrack";
}

bool Barrack::place(Board & board,uint16_t pos_x,uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    spice -= this->spice;
    energy -= this->energy;
    return true;
}

Refinery::Refinery(uint16_t LP,uint16_t spice,uint16_t energy, Position pos, uint8_t dim_x,uint8_t dim_y, uint16_t c_spice)
:
Building(LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Refinery";
    this->c_spice= c_spice;
}

bool Refinery::place(Board & board,uint16_t pos_x,uint16_t pos_y,uint16_t & spice,uint16_t & spice_capacity,int & energy,uint16_t & energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    spice -= this->spice;
    energy -= this->energy;
    spice_capacity += this->c_spice; 
    return true;
}

