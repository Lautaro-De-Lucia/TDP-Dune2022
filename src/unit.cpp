#include "unit.h"
#include "board.h"

Unit::Unit(player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed) 
:
Selectable(faction, LP,pos,dim_x,dim_y,true)
{
    this->spice = spice;
    this->speed = speed;
}

bool Unit::place(Board& board,std::vector<Position>& positions,int& spice){
    if ((spice - this->spice) < 0){
        //  Refactor: Manejarlo con excepciones
        std::cout << "Not enough Spice!!!" << std::endl;
        return false;
    }
    for (Position position : positions){
        if (board.place(position,1,1,this->faction) == SUCCESS){
            this->setPosition(position);
            spice -= this->spice;
            return true;
        }
    }
    return false;
}


void Unit::react(int x, int y, Board& board){
    std::cout << "reacting to cell in location: " << board.getCell(x,y).getPosition() << " [unit]" << std::endl;
}

Harvester::Harvester(player_t faction,int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int stored_spice, int max_spice) 
:
Unit(faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->stored_spice = stored_spice;
    this->max_spice = max_spice;
}

void Harvester::react(int x, int y, Board& board) {

    if (!board.getCell(x,y).isOccupied()) {
        aStar aStar;
        std::vector<Position> new_path = aStar.algorithm(this->getPosition(),Position(x,y),board);

        /*
        std::cout << "printing path" << '\n';
        for (size_t i = 0; i < new_path.size(); i++) {
            std::cout << new_path[i] << '\n';
        }
        */

        this->remaining_path = new_path;
    }
}

Trike::Trike(player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack)
:
Unit(faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack = attack;
}

void Trike::react(int x, int y, Board& board) {

    Cell& location = board.getCell(x,y);

    if (location.isOccupied())
        return;
    if (!location.canTraverse())        
        return;    
    //  FALTA LÓGICA DE ATAQUE
    this->move(x,y,board);
}

void Trike::move(int x, int y, Board& board) {
    //  Get path from A-Star algorithm
    //  The unit will move in accordance to this path
    aStar aStar;
    std::vector<Position> new_path = aStar.algorithm(this->getPosition(),Position(x,y),board);
    this->remaining_path = new_path;
}
