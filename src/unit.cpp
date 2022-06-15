#include "unit.h"
#include "board.h"

Unit::Unit(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed) 
:
Selectable(ID,faction, LP,pos,dim_x,dim_y,true)
{
    this->spice = spice;
    this->speed = speed;
    this->moving = false;
    this->current_time = 0;
    this->movement_time = std::round(20/speed);
}

bool Unit::place(Board& board,std::vector<Position>& positions,int * spice){
    std::cout << "Al menos nunca entro acá" << std::endl;
    return true;
}

bool Harvester::place(Board& board,std::vector<Position>& positions,int * spice){
    std::cout << this->spice << std::endl;
    std::cout << *spice << std::endl;
    if ((*spice - this->spice) < 0){
        //  Refactor: Manejarlo con excepciones
        std::cout << "Not enough Spice!!!" << std::endl;
        return false;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->faction,HARVESTER);
            *spice -= this->spice;
            this->player_spice = spice;
            return true;
        }
    }
    return false;
}

bool Trike::place(Board& board,std::vector<Position>& positions,int * spice){
    std::cout << this->spice << std::endl;
    std::cout << spice << std::endl;
    if ((*spice - this->spice) < 0){
        //  Refactor: Manejarlo con excepciones
        std::cout << "Not enough Spice!!!" << std::endl;
        return false;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->faction,TRIKE);
            *spice -= this->spice;
            return true;
        }
    }
    return false;
}

void Unit::react(int x, int y, Board& board){}
void Unit::update(State& state, Board& board){}

void Unit::move(int x, int y, Board& board) {
    std::vector<Position> new_path;
    if (Position(x,y) == this->getPosition()) {
        this->remaining_path = new_path;
        return;
    }
    std::cout << "Moving to that position" << std::endl;
    this->moving = true;
    aStar aStar;
    new_path = aStar.algorithm(this->getPosition(),Position(x,y),board);
    this->remaining_path = new_path;
    if(new_path.size() == 0)
        this->moving == false;
}

Harvester::Harvester(int ID,player_t faction,int LP,int spice, Position pos, int dim_x, int dim_y,int speed, int max_spice) 
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->stored_spice = 0;
    this->max_spice = max_spice;
    this->harvesting = false;
}

void Harvester::react(int x, int y, Board& board) {
    Cell& location = board.getCell(x,y);
    
    if (location.canDeposit(this->faction) && this->stored_spice > 0 ){
        this->deposit(x,y,board);
        return;
    }
    if (location.canHarvest() && this->stored_spice < this->max_spice){     
        this->harvest(x,y,board);
        return;
    }
    if (!location.canTraverse())        
        return;    

    this->harvesting = false;
    this->depositing = false;
    this->move(x,y,board);
}

void Harvester::harvest(int x, int y, Board& board){
    this->harvesting = true;
    this->harvest_position.x = x;
    this->harvest_position.y = y;
    this->move(x,y,board);
}


void Harvester::update(State& state, Board& board){
    //  UPDATE MOVEMENT
    if(this->moving == false){
        if (this->depositing == true){
            if (this->position == this->deposit_position){
                *(this->player_spice)+=this->stored_spice;
                this->stored_spice = 0;
                this->depositing = false;
                this->moving = false;
                this->harvesting = true;
            } else {
                    this->deposit(board);
            }
        }
        if(this->harvesting == true){
            if(this->position == this->harvest_position){
                Cell& harvestcell = board.getCell(harvest_position.x,harvest_position.y);
                int spice = harvestcell.extractSpice();
                if(spice == 0){
                    this->harvesting = false;
                    if(this->stored_spice > 0)
                        this->deposit(board);
                    else
                        return;    
                }
                this->stored_spice += spice;
                if(this->stored_spice == this->max_spice){
                    this->harvesting = false;
                    this->deposit(board);
                    return;
                }
            } else {
                if(board.getCell(harvest_position.x,harvest_position.y).isOccupied()){
                    return;
                }
                this->harvest(harvest_position.x,harvest_position.y,board);
            }
        } 
    }
    if(this->moving == true){
        if (this->harvesting == true){
            if(board.get_distance_between(this->harvest_position,this->position) <= 2){
                if(board.getCell(harvest_position.x,harvest_position.y).isOccupied()){
                    this->moving == false;
                    return;
                }
            }
        } 
        if (this->depositing == true){
            if(board.get_distance_between(this->position,this->deposit_position) <= 2){
                  if(board.getCell(deposit_position.x,deposit_position.y).isOccupied()){
                    this->moving == false;
                    return;
                }
            }
        }
        this->current_time++;
        if (this->remaining_path.size() == 0) {
            this->moving = false;
        } else if (this->current_time == this->movement_time) {
            this->current_time = 0;
            Position next = this->remaining_path.back();
            if(!(board.getCell(next.x,next.y).canTraverse())) {
                if (this->remaining_path.size() <= 1) {
                    std::vector<Position> empty_path;
                    this->remaining_path = empty_path;
                } else {
                    this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                }
            }
            if (this->remaining_path.size() != 0) {
                board.getCell(this->position.x,this->position.y).disoccupy();
                this->position = this->remaining_path.back();
                this->occupy(board);
                this->remaining_path.pop_back();
            }
        }
        if(remaining_path.size() == 0)
            this->moving = false;
    }
    //  UPDATE STATE
    Selectable::update(state,board);   
}

void Harvester::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->faction,HARVESTER);
}

void Harvester::deposit(Board & board){
    size_t best_distance = 1000;
    Position best_position;
    for (Position pos : board.getDepositPositions()){
        if (board.getCell(pos.x,pos.y).isOccupied())
            continue;
        size_t distance = board.get_distance_between(this->position,pos);
        if(distance < best_distance){
            best_distance = distance;
            best_position = pos;
        }
    }
    this->deposit_position = best_position;
    this->depositing = true;
    std::cout << "Now I'm going to deposit this" << std::endl;
    this->move(this->deposit_position.x,this->deposit_position.y,board);
}

void Harvester::deposit(int x, int y,Board & board){
    size_t best_distance = 1000;
    Position refinery_position(x,y);
    Position best_position;
    for (Position pos : board.getDepositPositions()){
        size_t distance = board.get_distance_between(refinery_position,pos);
        if(distance < best_distance){
            best_distance = distance;
            best_position = pos;
        }
    }
    this->deposit_position = best_position;
    this->depositing = true;
    this->move(this->deposit_position.x,this->deposit_position.y,board);
}

Trike::Trike(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack)
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
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
    if (this->remaining_path.size() == 0) {
        this->moving = false;
    } else if (this->current_time == this->movement_time) {
        this->current_time = 0;
        Position next = this->remaining_path.back();
        if(!(board.getCell(next.x,next.y).canTraverse())) {
            if (this->remaining_path.size() <= 1) {
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
            }
        }
        if (this->remaining_path.size() != 0) {
            board.getCell(this->position.x,this->position.y).disoccupy();
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
    if(this->remaining_path.size() == 0)
        this->moving = false;
    //  UPDATE STATE
    Selectable::update(state,board);
}

void Trike::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->faction,TRIKE);
}