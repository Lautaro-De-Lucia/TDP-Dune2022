#include "unit.h"
#include "board.h"

Unit::Unit(player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed) 
:
Selectable(faction, LP,pos,dim_x,dim_y,true)
{
    this->spice = spice;
    this->speed = speed;
    this->moving = false;
    this->current_time = 0;
    this->movement_time = std::round(20/speed);
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
void Unit::update(State& state, Board& board){
    
}


void Unit::move(int x, int y, Board& board) {
    std::cout << "Unit moving..." << std::endl;
    this->moving = true;
    aStar aStar;
    std::vector<Position> new_path = aStar.algorithm(this->getPosition(),Position(x,y),board);
    this->remaining_path = new_path;
}

Harvester::Harvester(player_t faction,int LP,int spice, Position pos, int dim_x, int dim_y,int speed, int max_spice) 
:
Unit(faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->stored_spice = 0;
    this->max_spice = max_spice;
    this->harvesting = false;
}

void Harvester::react(int x, int y, Board& board) {

    Cell& location = board.getCell(x,y);

    if (!location.canTraverse())        
        return;    
    if (location.canHarvest() && this->stored_spice < this->max_spice){     
        this->harvest(x,y,board);
        return;
    }
    this->move(x,y,board);
}

void Harvester::harvest(int x, int y, Board& board){
    std::cout << "Imma harvest" << std::endl;
    this->harvesting = true;
    this->harvest_position.x = x;
    this->harvest_position.y = y;
    this->move(x,y,board);
}


void Harvester::update(State& state, Board& board){
    //  UPDATE MOVEMENT
    if(this->moving == false){
        if(this->harvesting == true){
            if(this->position == this->harvest_position){
                Cell& harvestcell = board.getCell(harvest_position.x,harvest_position.y);
                int spice = harvestcell.extractSpice();
                if(spice == 0){
                    this->harvesting = false;
                    return;
                }
                if(this->stored_spice == this->max_spice){
                    this->harvesting = false;
                    return;
                }
            } else {
                this->harvesting == false;
            }
        } 
        return;
    }    
    this->current_time++;
    if (this->current_time == this->movement_time) {
        this->current_time = 0;
        Position next = this->remaining_path.back();
        if(!board.getCell(next.x,next.y).canTraverse())
            this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);		
        board.getCell(this->position.x,this->position.y).disoccupy();
        this->position = this->remaining_path.back();
        board.getCell(this->position.x,this->position.y).occupy(this->faction);
        this->remaining_path.pop_back();
    }
    if(remaining_path.size() == 0)
        this->moving = false;
    //  UPDATE STATE
    Selectable::update(state,board);
}

Trike::Trike(player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack)
:
Unit(faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack = attack;
}

void Trike::react(int x, int y, Board& board) {
    std::cout << "Trike: reacting " << std::endl;

    Cell& location = board.getCell(x,y);

    if (!location.canTraverse())        
        return;    
    //  FALTA LÓGICA DE ATAQUE
    this->move(x,y,board);
}

void Trike::update(State & state, Board& board){
    //  UPDATE MOVEMENT
    if(this->moving == false)
        return;

    this->current_time++;
    if (this->current_time == this->movement_time) {
        this->current_time = 0;
        Position next = this->remaining_path.back();
        if(!board.getCell(next.x,next.y).canTraverse())
            this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);		
        board.getCell(this->position.x,this->position.y).disoccupy();
        this->position = this->remaining_path.back();
        board.getCell(this->position.x,this->position.y).occupy(this->faction);
        this->remaining_path.pop_back();
    }
    
    if(remaining_path.size() == 0)
        this->moving = false;

    //  UPDATE STATE
    Selectable::getState(state);
}
