#include "unit.h"

Unit::Unit(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed) 
:
Selectable(LP,pos,dim_x,dim_y)
{
    this->spice = spice;
    this->speed = speed;
}

bool Unit::place(Board & board,std::vector<Position> & positions,uint16_t & spice){
    if ((spice - this->spice) < 0){
        std::cout << "Not enough Spice!!!" << std::endl;
        return false;
    }
    for (Position position : positions){
        if (board.place(position,1,1) == SUCCESS){
            this->setPosition(position);
            spice -= this->spice;
            return true;
        }
    }
    return false;
}


Harvester::Harvester(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed,uint16_t stored_spice, uint16_t max_spice) 
:
Unit(LP,spice,pos,dim_x,dim_y,speed)
{
    this->stored_spice = stored_spice;
    this->max_spice = max_spice;
}

Trike::Trike(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed,uint16_t attack)
:
Unit(LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack = attack;
}