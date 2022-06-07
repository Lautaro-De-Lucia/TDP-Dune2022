#include "unit.h"

#include "board.h"

/*
Unit::Unit(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed) 
:
Selectable(LP,pos,dim_x,dim_y,true)
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
        if (board.place(position,1,1,true) == SUCCESS){
            this->setPosition(position);
            spice -= this->spice;
            return true;
        }
    }
    return false;
}


void Unit::react(Cell & location){
    std::cout << "reacting to cell in location: " << location.getPosition() << " [unit]" << std::endl;
}

Unit::~Unit() {
    
}
*/


/*
Harvester::Harvester(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed,uint16_t stored_spice, uint16_t max_spice) 
:
Unit(LP,spice,pos,dim_x,dim_y,speed)
{
    this->stored_spice = stored_spice;
    this->max_spice = max_spice;
}
*/
Trike::Trike(uint16_t LP,uint16_t spice, Position pos, uint8_t dim_x, uint8_t dim_y,uint16_t speed,uint16_t attack)
:
Selectable(LP,pos,dim_x,dim_y,true)
{
    this->spice = spice;
    this->speed = speed;
    this->attack = attack;
}


void Trike::react(Cell& location) {

    std::cout << "Trike::react!!!!!!!!!!" << '\n';

    if(!location.isOccupied()) {
        aStar aStar;
        std::vector<Position> new_path = aStar.algorithm(this->getPosition(), location.getPosition());

        /*
        std::cout << "printing path" << '\n';
        for (size_t i = 0; i < new_path.size(); i++) {
            std::cout << new_path[i] << '\n';
        }
        */

        this->remaining_path = new_path;
    }
}

bool Trike::place(Board & board,std::vector<Position> & positions,uint16_t & spice){
    if ((spice - this->spice) < 0){
        std::cout << "Not enough Spice!!!" << std::endl;
        return false;
    }
    for (Position position : positions){
        if (board.place(position,1,1,true) == SUCCESS){
            this->setPosition(position);
            spice -= this->spice;
            return true;
        }
    }
    return false;
}

Trike::~Trike() {
    
}
