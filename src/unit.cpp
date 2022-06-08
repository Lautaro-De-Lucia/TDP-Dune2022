#include "unit.h"
#include "board.h"

Unit::Unit(int LP,int spice, Position pos, int dim_x, int dim_y,int speed) 
:
Selectable(LP,pos,dim_x,dim_y,true)
{
    this->spice = spice;
    this->speed = speed;
}

bool Unit::place(Board & board,std::vector<Position> & positions,int & spice){
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

Harvester::Harvester(int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int stored_spice, int max_spice) 
:
Unit(LP,spice,pos,dim_x,dim_y,speed)
{
    this->stored_spice = stored_spice;
    this->max_spice = max_spice;
}

void Harvester::react(Cell& location) {

    if (!location.isOccupied()) {
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

Trike::Trike(int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack)
:
Unit(LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack = attack;
}

void Trike::react(Cell& location) {

    if (!location.isOccupied()) {
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
