#include "server_unit.h"
#include "server_board.h"

Unit::Unit(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed) 
:
Selectable(ID,faction, LP,pos,dim_x,dim_y,true)
{
    this->spice = spice;
    this->speed = speed;
    this->direction = BOTTOM;
    this->moving = false;
    this->waiting = false;
    this->current_time = 0;
    this->movement_time = TILE_SIZE;
}

response_t Unit::place(Board& board,std::vector<Position>& positions,int * spice){
    return RES_SUCCESS;
}

void Unit::sendState(Protocol & protocol, Socket & client_socket){
}

void Unit::react(int x, int y, Board& board){}
void Unit::update(State& state, Board& board){}
void Unit::receiveDamage(int damage){}


void Unit::move(int x, int y, Board& board) {
    std::vector<Position> empty_path;
    std::vector<Position> new_path;
    //std::cout << "Moving to position: " << Position(x,y) << std::endl;
    if (Position(x,y) == this->getPosition()) {
        this->remaining_path = empty_path;
        return;
    }
    this->moving = true;
    aStar aStar;
    new_path = aStar.algorithm(this->getPosition(),Position(x,y),board);

    if(new_path.size() <= 1){ 
        std::cerr << "I shouldn't be here" << std::endl; // why not?
        this->moving == false;
        this->remaining_path = empty_path;
        return;
    }

    // first position is redundant
    new_path.pop_back();
    this->remaining_path = new_path;
}

Harvester::Harvester(int ID,player_t faction,int LP,int spice, Position pos, int dim_x, int dim_y,int speed, int max_spice) 
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->stored_spice = 0;
    this->max_spice = max_spice;
    this->harvesting = false;
}

response_t Harvester::place(Board& board,std::vector<Position>& positions,int * spice){
    if ((*spice - this->spice) < 0){
        return RES_CREATE_UNIT_FAILURE_SPICE;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->ID);
            *spice -= this->spice;
            this->player_spice = spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}

void Harvester::sendState(Protocol & protocol, Socket & client_socket){
    protocol.send_harvester(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        this->spice,
        this->harvesting,
        client_socket); 
}

void Harvester::react(int x, int y, Board& board) {
    
    if (board.canDeposit(x,y,this->faction)){
        this->deposit(x,y,board);
        return;
    }
    if (board.canHarvest(x,y) && this->stored_spice < this->max_spice){     
        this->harvest(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
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

void Harvester::receiveDamage(int damage){
    this->LP = this->LP-damage;
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
                board.addSandPosition(harvest_position.x,harvest_position.y);
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
        this->current_time+=this->speed;
        if (this->remaining_path.size() == 0) {
            this->moving = false;
        } else if (this->current_time >= this->movement_time) {
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
    board.getCell(this->position.x,this->position.y).occupy(this->ID);
}

void Harvester::deposit(Board & board){
    size_t best_distance = 1000;
    Position best_position;
    for (Position pos : board.getDepositPositions(this->faction)){
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
    this->move(this->deposit_position.x,this->deposit_position.y,board);
}

void Harvester::deposit(int x, int y,Board & board) {
    size_t best_distance = 1000;
    Position refinery_position(x,y);
    Position best_position;
    for (Position pos : board.getDepositPositions(this->faction)){
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

int Harvester::getSpice() {
    return this->stored_spice;
}

bool Harvester::isHarvesting() {
    return this->harvesting;
}

Trike::Trike(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack_points = attack;
    this->range = range;
    this->direction = BOTTOM;
    this->speed = speed;
    this->moving = false;
    this->targeting = false;
    this->attacking = false;
}

void Trike::react(int x, int y, Board& board) {
    //std::cout << "Trike reacting! " << std::endl;
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;

    this->pending_move.push(Position(x, y));        
}

void Trike::attack(int x, int y, Board& board){
    this->targeting = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;

    Position selectable_pos = (board.getElementAt(x,y))->getPosition();
    int selectable_dim_x = (board.getElementAt(x,y))->getDimX();
    int selectable_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(selectable_pos, selectable_dim_x, selectable_dim_y); //  FAILING HERE

    for (Position pos : attacking_positions)
        if(board.canTraverse(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        }  
    this->move(this->moving_position.x,this->moving_position.y,board);
}

void Trike::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

bool Trike::enemySearch(Board & board){
    for (size_t i = 0 ; i < board.getDimX() ; i++){
        for (size_t j = 0 ; j < board.getDimY() ; j++){
            if(board.get_distance_between(Position(i,j),this->position) < this->range){
                if(board.hasEnemy(i,j,this->faction)){
                    this->enemy_position = Position(i,j);
                    return true;
                }
            }
        }
    }
    return false;
}

void Trike::update(State & state, Board& board){

    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
    //  UPDATE MOVEMENT
    if(this->moving == false && this->targeting == false){
		if (this->enemySearch(board) == true)
		    this->attack(this->enemy_position.x,this->enemy_position.y,board);
    }
    if (this->targeting == true){
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            if(!board.canTraverse(this->moving_position.x,this->moving_position.y))
                if(this->position.x != this->moving_position.x || this->position.y != this->moving_position.y)
                    this->attack(this->enemy_position.x,this->enemy_position.y,board);
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                this->attacking = true;
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            this->moving = false;
            this->targeting = false;
            this->attacking = false;	
            return;
        }
    }
    if(this->moving == true){
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        if (this->remaining_path.size() != 0) {
            //std::cout << "It is moving with the direction: "<< this->direction << std::endl;
            Position next_position = remaining_path.back();
            if (next_position.x == this->position.x && next_position.y > this->position.y)
                this->direction = BOTTOM;
            if (next_position.x > this->position.x && next_position.y > this->position.y)
                this->direction = BOTTOM_RIGHT;    
            if (next_position.x > this->position.x && next_position.y == this->position.y)
                this->direction = RIGHT;
            if (next_position.x > this->position.x && next_position.y < this->position.y)
                this->direction = TOP_RIGHT;
            if (next_position.x == this->position.x && next_position.y < this->position.y)
                this->direction = TOP;
            if (next_position.x < this->position.x && next_position.y < this->position.y)
                this->direction = TOP_LEFT; 
            if (next_position.x < this->position.x && next_position.y == this->position.y)
                this->direction = LEFT;
            if (next_position.x < this->position.x && next_position.y > this->position.y)
                this->direction = BOTTOM_LEFT;
            this->current_time+=this->speed;
        }
        if(this->remaining_path.size() != 0){
            Position next = this->remaining_path.back();
            if(!(board.getCell(next.x,next.y).canTraverse())){
                this->waiting = true;
            } else {
                this->waiting = false;
            }
        }
        //if(this->waiting == true){
        //    Position next = this->remaining_path.back();
        //        if(board.getCell(next.x,next.y).canTraverse())
        //            this->waiting = false;
        //}
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
        } else if (this->current_time >= this->movement_time) {
            this->current_time = 0;
            Position next = this->remaining_path.back();
            if(!(board.getCell(next.x,next.y).canTraverse())) {
                if (this->remaining_path.size() <= 1) {
                    std::cout << this->ID << ": Staying here..." << std::endl;
          //          this->waiting = true;
                    std::vector<Position> empty_path;
                    this->remaining_path = empty_path;
                } else {
                    std::cout << this->ID << ": Recalculating path..." << std::endl;
          //        this->waiting = true;
                    this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                    Position next_position = remaining_path.back();
                    if (next_position.x == this->position.x && next_position.y > this->position.y)
                        this->direction = BOTTOM;
                    if (next_position.x > this->position.x && next_position.y > this->position.y)
                        this->direction = BOTTOM_RIGHT;    
                    if (next_position.x > this->position.x && next_position.y == this->position.y)
                        this->direction = RIGHT;
                    if (next_position.x > this->position.x && next_position.y < this->position.y)
                        this->direction = TOP_RIGHT;
                    if (next_position.x == this->position.x && next_position.y < this->position.y)
                        this->direction = TOP;
                    if (next_position.x < this->position.x && next_position.y < this->position.y)
                        this->direction = TOP_LEFT; 
                    if (next_position.x < this->position.x && next_position.y == this->position.y)
                        this->direction = LEFT;
                    if (next_position.x < this->position.x && next_position.y > this->position.y)
                        this->direction = BOTTOM_LEFT;
                }
            }
            if (this->remaining_path.size() != 0) {
                    board.getCell(this->position.x,this->position.y).disoccupy();
                    this->position = this->remaining_path.back();
                    this->occupy(board);
                    this->remaining_path.pop_back();
            } 
        }
    }
    //std::cout << "But before leaving this function, this direction will have changed to: " << this->direction <<std::endl;
    Selectable::update(state,board);
}

void Trike::sendState(Protocol & protocol, Socket & client_socket){
    //std::cout << "Now this direction: " << this->direction << " Will be sent" << std::endl;
    protocol.send_trike(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->direction,
        this->moving,
        this->selected,
        this->attacking,
        this->waiting,
        client_socket); 
}


void Trike::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->ID);
}

bool Trike::isAttacking() {
    return this->attacking;
}

response_t Trike::place(Board& board,std::vector<Position>& positions,int * spice){
    if ((*spice - this->spice) < 0){
        return RES_CREATE_UNIT_FAILURE_SPICE;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}




Fremen::Fremen(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack_points = attack;
    this->range = range;
    this->moving = false;
    this->attacking = false;
}

void Fremen::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;    
    this->move(x,y,board);
}

void Fremen::attack(int x, int y, Board& board){
    this->attacking = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;

    Position selectable_pos = (board.getElementAt(x,y))->getPosition();
    int selectable_dim_x = (board.getElementAt(x,y))->getDimX();
    int selectable_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(selectable_pos, selectable_dim_x, selectable_dim_y); //  FAILING HERE

    for (Position pos : attacking_positions)
        if(board.canTraverse(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        }  
    this->move(this->moving_position.x,this->moving_position.y,board);
}

void Fremen::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

bool Fremen::enemySearch(Board & board){
    for (size_t i = 0 ; i < board.getDimX() ; i++){
        for (size_t j = 0 ; j < board.getDimY() ; j++){
            if(board.get_distance_between(Position(i,j),this->position) < this->range){
                if(board.hasEnemy(i,j,this->faction)){
                    this->enemy_position = Position(i,j);
                    return true;
                }
            }
        }
    }
    return false;
}

void Fremen::update(State & state, Board& board){

    //  UPDATE MOVEMENT
    if(this->moving == false && this->attacking == false){
		if (this->enemySearch(board) == true)
		    this->attack(this->enemy_position.x,this->enemy_position.y,board);
    }
    if (this->attacking == true){
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            if(!board.canTraverse(this->moving_position.x,this->moving_position.y))
                if(this->position.x != this->moving_position.x || this->position.y != this->moving_position.y)
                    this->attack(this->enemy_position.x,this->enemy_position.y,board);
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            this->moving = false;
            this->attacking = false;	
            return;
        }
    }
    if(this->moving == true){
        this->current_time+=this->speed;
        if (this->remaining_path.size() == 0) {
            this->moving = false;
        } else if (this->current_time >= this->movement_time) {
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
    }
    Selectable::update(state,board);
}

void Fremen::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->ID);
}

bool Fremen::isAttacking() {
    return this->attacking;
}

response_t Fremen::place(Board& board,std::vector<Position>& positions,int * spice){
    if ((*spice - this->spice) < 0){
        return RES_CREATE_UNIT_FAILURE_SPICE;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}


void Fremen::sendState(Protocol & protocol, Socket & client_socket){
    protocol.send_fremen(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        this->attacking,
        client_socket); 
}

Infantry::Infantry(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack_points = attack;
    this->range = range;
    this->moving = false;
    this->attacking = false;
}

void Infantry::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;    
    this->move(x,y,board);
}

void Infantry::attack(int x, int y, Board& board){
    this->attacking = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;

    Position selectable_pos = (board.getElementAt(x,y))->getPosition();
    int selectable_dim_x = (board.getElementAt(x,y))->getDimX();
    int selectable_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(selectable_pos, selectable_dim_x, selectable_dim_y); //  FAILING HERE

    for (Position pos : attacking_positions)
        if(board.canTraverse(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        }  
    this->move(this->moving_position.x,this->moving_position.y,board);
}

void Infantry::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

bool Infantry::enemySearch(Board & board){
    for (size_t i = 0 ; i < board.getDimX() ; i++){
        for (size_t j = 0 ; j < board.getDimY() ; j++){
            if(board.get_distance_between(Position(i,j),this->position) < this->range){
                if(board.hasEnemy(i,j,this->faction)){
                    this->enemy_position = Position(i,j);
                    return true;
                }
            }
        }
    }
    return false;
}

void Infantry::update(State & state, Board& board){

    //  UPDATE MOVEMENT
    if(this->moving == false && this->attacking == false){
		if (this->enemySearch(board) == true)
		    this->attack(this->enemy_position.x,this->enemy_position.y,board);
    }
    if (this->attacking == true){
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            if(!board.canTraverse(this->moving_position.x,this->moving_position.y))
                if(this->position.x != this->moving_position.x || this->position.y != this->moving_position.y)
                    this->attack(this->enemy_position.x,this->enemy_position.y,board);
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            this->moving = false;
            this->attacking = false;	
            return;
        }
    }
    if(this->moving == true){
        this->current_time+=this->speed;
        if (this->remaining_path.size() == 0) {
            this->moving = false;
        } else if (this->current_time >= this->movement_time) {
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
    }
    Selectable::update(state,board);
}

void Infantry::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->ID);
}

bool Infantry::isAttacking() {
    return this->attacking;
}

response_t Infantry::place(Board& board,std::vector<Position>& positions,int * spice){
    if ((*spice - this->spice) < 0){
        return RES_CREATE_UNIT_FAILURE_SPICE;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}


void Infantry::sendState(Protocol & protocol, Socket & client_socket){
    protocol.send_infantry(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        this->attacking,
        client_socket); 
}


Sardaukar::Sardaukar(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack_points = attack;
    this->range = range;
    this->moving = false;
    this->attacking = false;
}

void Sardaukar::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;    
    this->move(x,y,board);
}

void Sardaukar::attack(int x, int y, Board& board){
    this->attacking = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;

    Position selectable_pos = (board.getElementAt(x,y))->getPosition();
    int selectable_dim_x = (board.getElementAt(x,y))->getDimX();
    int selectable_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(selectable_pos, selectable_dim_x, selectable_dim_y); //  FAILING HERE

    for (Position pos : attacking_positions)
        if(board.canTraverse(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        }  
    this->move(this->moving_position.x,this->moving_position.y,board);
}

void Sardaukar::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

bool Sardaukar::enemySearch(Board & board){
    for (size_t i = 0 ; i < board.getDimX() ; i++){
        for (size_t j = 0 ; j < board.getDimY() ; j++){
            if(board.get_distance_between(Position(i,j),this->position) < this->range){
                if(board.hasEnemy(i,j,this->faction)){
                    this->enemy_position = Position(i,j);
                    return true;
                }
            }
        }
    }
    return false;
}

void Sardaukar::update(State & state, Board& board){

    //  UPDATE MOVEMENT
    if(this->moving == false && this->attacking == false){
		if (this->enemySearch(board) == true)
		    this->attack(this->enemy_position.x,this->enemy_position.y,board);
    }
    if (this->attacking == true){
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            if(!board.canTraverse(this->moving_position.x,this->moving_position.y))
                if(this->position.x != this->moving_position.x || this->position.y != this->moving_position.y)
                    this->attack(this->enemy_position.x,this->enemy_position.y,board);
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            this->moving = false;
            this->attacking = false;	
            return;
        }
    }
    if(this->moving == true){
        this->current_time+=this->speed;
        if (this->remaining_path.size() == 0) {
            this->moving = false;
        } else if (this->current_time >= this->movement_time) {
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
    }
    Selectable::update(state,board);
}

void Sardaukar::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->ID);
}

bool Sardaukar::isAttacking() {
    return this->attacking;
}

response_t Sardaukar::place(Board& board,std::vector<Position>& positions,int * spice){
    if ((*spice - this->spice) < 0){
        return RES_CREATE_UNIT_FAILURE_SPICE;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}


void Sardaukar::sendState(Protocol & protocol, Socket & client_socket){
    protocol.send_sardaukar(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        this->attacking,
        client_socket); 
}


Tank::Tank(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack_points = attack;
    this->range = range;
    this->moving = false;
    this->attacking = false;
}

void Tank::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;    
    this->move(x,y,board);
}

void Tank::attack(int x, int y, Board& board){
    this->attacking = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;

    Position selectable_pos = (board.getElementAt(x,y))->getPosition();
    int selectable_dim_x = (board.getElementAt(x,y))->getDimX();
    int selectable_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(selectable_pos, selectable_dim_x, selectable_dim_y); //  FAILING HERE

    for (Position pos : attacking_positions)
        if(board.canTraverse(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        }  
    this->move(this->moving_position.x,this->moving_position.y,board);
}

void Tank::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

bool Tank::enemySearch(Board & board){
    for (size_t i = 0 ; i < board.getDimX() ; i++){
        for (size_t j = 0 ; j < board.getDimY() ; j++){
            if(board.get_distance_between(Position(i,j),this->position) < this->range){
                if(board.hasEnemy(i,j,this->faction)){
                    this->enemy_position = Position(i,j);
                    return true;
                }
            }
        }
    }
    return false;
}

void Tank::update(State & state, Board& board){

    //  UPDATE MOVEMENT
    if(this->moving == false && this->attacking == false){
		if (this->enemySearch(board) == true)
		    this->attack(this->enemy_position.x,this->enemy_position.y,board);
    }
    if (this->attacking == true){
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            if(!board.canTraverse(this->moving_position.x,this->moving_position.y))
                if(this->position.x != this->moving_position.x || this->position.y != this->moving_position.y)
                    this->attack(this->enemy_position.x,this->enemy_position.y,board);
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            this->moving = false;
            this->attacking = false;	
            return;
        }
    }
    if(this->moving == true){
        this->current_time+=this->speed;
        if (this->remaining_path.size() == 0) {
            this->moving = false;
        } else if (this->current_time >= this->movement_time) {
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
    }
    Selectable::update(state,board);
}

void Tank::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->ID);
}

bool Tank::isAttacking() {
    return this->attacking;
}

response_t Tank::place(Board& board,std::vector<Position>& positions,int * spice){
    if ((*spice - this->spice) < 0){
        return RES_CREATE_UNIT_FAILURE_SPICE;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}


void Tank::sendState(Protocol & protocol, Socket & client_socket){
    protocol.send_tank(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        this->attacking,
        client_socket); 
}


Devastator::Devastator(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
:
Unit(ID,faction,LP,spice,pos,dim_x,dim_y,speed)
{
    this->attack_points = attack;
    this->range = range;
    this->moving = false;
    this->attacking = false;
}

void Devastator::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;    
    this->move(x,y,board);
}

void Devastator::attack(int x, int y, Board& board){
    this->attacking = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;

    Position selectable_pos = (board.getElementAt(x,y))->getPosition();
    int selectable_dim_x = (board.getElementAt(x,y))->getDimX();
    int selectable_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(selectable_pos, selectable_dim_x, selectable_dim_y); //  FAILING HERE

    for (Position pos : attacking_positions)
        if(board.canTraverse(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        }  
    this->move(this->moving_position.x,this->moving_position.y,board);
}

void Devastator::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

bool Devastator::enemySearch(Board & board){
    for (size_t i = 0 ; i < board.getDimX() ; i++){
        for (size_t j = 0 ; j < board.getDimY() ; j++){
            if(board.get_distance_between(Position(i,j),this->position) < this->range){
                if(board.hasEnemy(i,j,this->faction)){
                    this->enemy_position = Position(i,j);
                    return true;
                }
            }
        }
    }
    return false;
}

void Devastator::update(State & state, Board& board){

    //  UPDATE MOVEMENT
    if(this->moving == false && this->attacking == false){
		if (this->enemySearch(board) == true)
		    this->attack(this->enemy_position.x,this->enemy_position.y,board);
    }
    if (this->attacking == true){
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            if(!board.canTraverse(this->moving_position.x,this->moving_position.y))
                if(this->position.x != this->moving_position.x || this->position.y != this->moving_position.y)
                    this->attack(this->enemy_position.x,this->enemy_position.y,board);
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            this->moving = false;
            this->attacking = false;	
            return;
        }
    }
    if(this->moving == true){
        this->current_time+=this->speed;
        if (this->remaining_path.size() == 0) {
            this->moving = false;
        } else if (this->current_time >= this->movement_time) {
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
    }
    Selectable::update(state,board);
}

void Devastator::occupy(Board & board){
    board.getCell(this->position.x,this->position.y).occupy(this->ID);
}

bool Devastator::isAttacking() {
    return this->attacking;
}

response_t Devastator::place(Board& board,std::vector<Position>& positions,int * spice){
    if ((*spice - this->spice) < 0){
        return RES_CREATE_UNIT_FAILURE_SPICE;
    }
    for (Position position : positions){
        if (board.canPlace(position,1,1) == SUCCESS){
            this->setPosition(position);
            board.getCell(position.x,position.y).occupy(this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}


void Devastator::sendState(Protocol & protocol, Socket & client_socket){
    protocol.send_devastator(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        this->attacking,
        client_socket); 
}