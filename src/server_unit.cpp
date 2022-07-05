#include "server_unit.h"

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

std::vector<Position> Unit::getPositions(){
    //std::cout << "Getting positions of the destroyed unit" << std::endl;
    std::vector<Position> positions;
    positions.push_back(this->position);
    if(this->next_position.x != -1 && this->next_position.y !=-1)
        positions.push_back(this->next_position);
    return positions;
}

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
        //std::cerr << "I shouldn't be here" << std::endl; // why not?
        this->moving == false;
        this->remaining_path = empty_path;
        return;
    }

    // first position is redundant
    new_path.pop_back();
    this->remaining_path = new_path;
}

void Unit::focus(Position & other_pos){
    if (other_pos.x == this->position.x && other_pos.y > this->position.y)
        this->direction = BOTTOM;
    if (other_pos.x > this->position.x && other_pos.y > this->position.y)
        this->direction = BOTTOM_RIGHT;    
    if (other_pos.x > this->position.x && other_pos.y == this->position.y)
        this->direction = RIGHT;
    if (other_pos.x > this->position.x && other_pos.y < this->position.y)
        this->direction = TOP_RIGHT;
    if (other_pos.x == this->position.x && other_pos.y < this->position.y)
        this->direction = TOP;
    if (other_pos.x < this->position.x && other_pos.y < this->position.y)
        this->direction = TOP_LEFT; 
    if (other_pos.x < this->position.x && other_pos.y == this->position.y)
        this->direction = LEFT;
    if (other_pos.x < this->position.x && other_pos.y > this->position.y)
        this->direction = BOTTOM_LEFT;
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
            board.occupy(position.x,position.y,this->ID);
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
        this->speed,
        this->direction,
        this->moving,
        this->selected,
        this->spice,
        this->harvesting,
        this->waiting,
        client_socket);    
}

void Harvester::react(int x, int y, Board& board) {
    
    if (board.canDeposit(x,y,this->faction) && this->stored_spice > 0){
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

    this->pending_move.push(Position(x, y));        
}

void Harvester::harvest(int x, int y, Board& board){
    this->harvesting = true;
    this->harvest_position.x = x;
    this->harvest_position.y = y;
    this->pending_move.push(Position(x, y));        
}

void Harvester::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

void Harvester::update(State& state, Board& board){
    
    if(board.canSlowDown(this->position.x,this->position.y)){
        this->speed = HARVESTER_SPEED/2;
    } else{
        this->speed = HARVESTER_SPEED;
    }

    Selectable::update(state,board);

    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
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
                int spice = board.extractSpice(harvest_position.x,harvest_position.y);
                board.addSandPosition(harvest_position.x,harvest_position.y);
                if(spice == 0){
                    if(this->stored_spice > 0){
                        this->deposit(board);
                    }
                    else{
                        for(Position pos : board.getSurroundings(this->position,1,1)){
                            if(board.canHarvest(pos.x,pos.y) && !board.isOccupied(pos.x,pos.y) && (board.getReserveID(pos.x,pos.y) == -1 || board.getReserveID(pos.x,pos.y) == this->ID)){
                                this->harvest(pos.x,pos.y,board);
                                return;
                            }
                        }
                        this->harvesting = false;
                        return;
                    }  
                }
                this->stored_spice += spice;
                if(this->stored_spice == this->max_spice){
                    this->harvesting = false;
                    this->deposit(board);
                    return;
                }
            } else {
                this->harvest(harvest_position.x,harvest_position.y,board);
            }
        } 
    }
    if(this->moving == true){
        //  Si el camino termino, dejar de moverse
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
            return;
        }
        if (this->harvesting == true){
            if(board.get_distance_between(this->harvest_position,this->position) <= 1){
                if(board.isOccupied(harvest_position.x,harvest_position.y) || (board.getReserveID(harvest_position.x,harvest_position.y) != -1 && board.getReserveID(harvest_position.x,harvest_position.y) != this->ID)){
                    this->moving = false;
                    this->waiting = true;
                    return;
                }
            }
        } 
        if (this->depositing == true){
            if(board.get_distance_between(this->position,this->deposit_position) <= 1){
                if(board.isOccupied(deposit_position.x,deposit_position.y) || (board.getReserveID(deposit_position.x,deposit_position.y) != -1 && board.getReserveID(deposit_position.x,deposit_position.y) != this->ID)){
                    this->moving = false;
                    this->waiting = true;
                    return;
                }
            }
        }
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        Position next = remaining_path.back();
        this->focus(next);
        //  Miro la próxima posición
        if(board.canTraverse(next.x,next.y) && (board.getReserveID(next.x,next.y) == -1 || board.getReserveID(next.x,next.y) == this->ID) && !board.isOccupied(next.x,next.y)){            //  Si puedo ir, ocuparla
            this->waiting = false;
            board.reserve(next.x,next.y,this->ID);
            this->next_position = next;          
            this->current_time+=this->speed;    //  Increase counter
        } else {
            //  Si no puedo ir 
            if (this->remaining_path.size() <= 1) {
                //  Me quedo
                this->waiting = true;
                this->moving = false;
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                //  Sino, recalculo el camino a destino
                this->waiting = true;
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                Position next_position = remaining_path.back();
                this->focus(next_position);
            }   
        }
        if (this->current_time >= this->movement_time) {
            //  Cuando se cumple el tiempo, cambiar de posición
            this->current_time = 0; //  Reset counter
            board.disoccupy(this->position.x,this->position.y);
            board.unReserve(this->next_position.x,this->next_position.y);
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
}

void Harvester::occupy(Board & board){
    board.occupy(this->position.x,this->position.y,this->ID);
}

void Harvester::deposit(Board & board){
    size_t best_distance = 1000;
    Position best_position;
    for (Position pos : board.getDepositPositions(this->faction)){
        if (board.isOccupied(pos.x,pos.y) || !board.canTraverse(pos.x,pos.y) || (board.getReserveID(pos.x,pos.y) != -1 && board.getReserveID(pos.x,pos.y) != this->ID))
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
    this->enemy_position.x = -1;
    this->enemy_position.y = -1;
}
void Trike::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;
    this->attacking = false;
    this->targeting =false;
    this->pending_move.push(Position(x, y));        
}

void Trike::attack(int x, int y, Board& board){
    this->targeting = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;
    //  Get the surrounding positions of my enemy
    int enemy_dim_x = (board.getElementAt(x,y))->getDimX();
    int enemy_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(this->enemy_position, enemy_dim_x, enemy_dim_y); 
    //  Find the first position that is neither occupied nor reserved
    for (Position pos : attacking_positions){
        if(board.canTraverse(pos.x,pos.y) && (board.getReserveID(pos.x,pos.y) == -1 || board.getReserveID(pos.x,pos.y) == this->ID) && !board.isOccupied(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        } 
    } 
    //std::cout << "Moving to the position: " << this->moving_position << std::endl;
    //  Move there
    this->pending_move.push(Position(this->moving_position.x,this->moving_position.y));
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
    Selectable::update(state,board);

    if(board.canSlowDown(this->position.x,this->position.y)){
        this->speed = TRIKE_SPEED/2;
    } else{
        this->speed = TRIKE_SPEED;
    }

    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
    //  Si no me estoy moviendo ni atacando, buscar enemigos
    if(this->moving == false && this->targeting == false){
		if (this->enemySearch(board) == true){
            //std::cout << "Attacking position: "<< this->enemy_position << std::endl;
            this->attack(this->enemy_position.x,this->enemy_position.y,board);
        }
    }
    //  Si estoy atacando, perseguir
    if (this->targeting == true){
        //  If there is still an enemy at the position
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            //  If he is on range, stop & shoot
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                this->focus(this->enemy_position);
                this->waiting = true;    
                this->attacking = true;
                this->moving = false;
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            //  If the enemy left the position. Stop (this will get the unit on searchEnemy() mode again)
            this->waiting = false;
            this->moving = false;
            this->targeting = false;
            this->attacking = false;
            this->enemy_position.x = -1;
            this->enemy_position.y = -1;	
            this->current_time = 0;
            return;
        }
    }
    //  Si me estoy moviendo
    if(this->moving == true){
        //  Si el camino termino, dejar de moverse
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
            return;
        }
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        Position next = remaining_path.back();
        this->focus(next);
        //  Miro la próxima posición
        if(board.canTraverse(next.x,next.y)  && (board.getReserveID(next.x,next.y) == -1 || board.getReserveID(next.x,next.y) == this->ID) && !board.isOccupied(next.x,next.y)){            //  Si puedo ir, ocuparla
            this->waiting = false;
            board.reserve(next.x,next.y,this->ID);
            this->next_position = next;    
            this->current_time+=this->speed;    //  Increase counter
        } else {
            //  Si no puedo ir 
            if (this->remaining_path.size() <= 1) {
                //  Me quedo
                this->waiting = true;
                this->moving = false;
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                //  Sino, recalculo el camino a destino
                this->waiting = true;
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                Position next_position = remaining_path.back();
                this->focus(next_position);
            }   
        }
        if (this->current_time >= this->movement_time) {
            //  Cuando se cumple el tiempo, cambiar de posición
            this->current_time = 0; //  Reset counter
            board.disoccupy(this->position.x,this->position.y);
            board.unReserve(this->next_position.x,this->next_position.y);
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
}

void Trike::sendState(Protocol & protocol, Socket & client_socket){
    //std::cout << "Now this direction: " << this->direction << " Will be sent" << std::endl;
    protocol.send_trike(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->speed,
        this->direction,
        this->moving,
        this->selected,
        this->attacking,
        this->enemy_position.x,
        this->enemy_position.y,
        this->waiting,
        client_socket); 
}


void Trike::occupy(Board & board){
    board.occupy(this->position.x,this->position.y,this->ID);
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
            board.occupy(position.x,position.y,this->ID);
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
    this->direction = BOTTOM;
    this->speed = speed;
    this->moving = false;
    this->targeting = false;
    this->attacking = false;
    this->enemy_position.x = -1;
    this->enemy_position.y = -1;
}
void Fremen::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;
    this->attacking = false;
    this->targeting =false;
    this->pending_move.push(Position(x, y));         
}

void Fremen::attack(int x, int y, Board& board){
    this->targeting = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;
    //  Get the surrounding positions of my enemy
    int enemy_dim_x = (board.getElementAt(x,y))->getDimX();
    int enemy_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(this->enemy_position, enemy_dim_x, enemy_dim_y); 
    //  Find the first position that is neither occupied nor reserved
    for (Position pos : attacking_positions){
        if(board.canTraverse(pos.x, pos.y)  && (board.getReserveID(pos.x,pos.y) == -1 || board.getReserveID(pos.x,pos.y) == this->ID) && !board.isOccupied(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        } 
    } 
    //std::cout << "Moving to the position: " << this->moving_position << std::endl;
    //  Move there
    this->pending_move.push(Position(this->moving_position.x,this->moving_position.y));
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
    Selectable::update(state,board);
    
    if(board.canSlowDown(this->position.x,this->position.y)){
        this->speed = FREMEN_SPEED/2;
    } else{
        this->speed = FREMEN_SPEED;
    }

    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
    //  Si no me estoy moviendo ni atacando, buscar enemigos
    if(this->moving == false && this->targeting == false){
		if (this->enemySearch(board) == true){
            //std::cout << "Attacking position: "<< this->enemy_position << std::endl;
            this->attack(this->enemy_position.x,this->enemy_position.y,board);
        }
    }
    //  Si estoy atacando, perseguir
    if (this->targeting == true){
        //  If there is still an enemy at the position
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            //  If he is on range, stop & shoot
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                this->focus(this->enemy_position);
                this->waiting = true;    
                this->attacking = true;
                this->moving = false;
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            //  If the enemy left the position. Stop (this will get the unit on searchEnemy() mode again)
            this->waiting = false;
            this->moving = false;
            this->targeting = false;
            this->attacking = false;
            this->enemy_position.x = -1;
            this->enemy_position.y = -1;	
            this->current_time = 0;
            return;
        }
    }
    //  Si me estoy moviendo
    if(this->moving == true){
        //  Si el camino termino, dejar de moverse
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
            return;
        }
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        Position next = remaining_path.back();
        this->focus(next);
        //  Miro la próxima posición
        if(board.canTraverse(next.x, next.y)  && (board.getReserveID(next.x,next.y) == -1 || board.getReserveID(next.x,next.y) == this->ID) && !board.isOccupied(next.x,next.y)){            //  Si puedo ir, ocuparla
            this->waiting = false;
            board.reserve(next.x,next.y,this->ID);
            this->next_position = next;          
            this->current_time+=this->speed;    //  Increase counter
        } else {
            //  Si no puedo ir 
            if (this->remaining_path.size() <= 1) {
                //  Me quedo
                this->waiting = true;
                this->moving = false;
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                //  Sino, recalculo el camino a destino
                this->waiting = true;
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                Position next_position = remaining_path.back();
                this->focus(next_position);
            }   
        }
        if (this->current_time >= this->movement_time) {
            //  Cuando se cumple el tiempo, cambiar de posición
            this->current_time = 0; //  Reset counter
            board.disoccupy(this->position.x,this->position.y);
            board.unReserve(this->next_position.x,this->next_position.y);
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
}

void Fremen::sendState(Protocol & protocol, Socket & client_socket){
    //std::cout << "Now this direction: " << this->direction << " Will be sent" << std::endl;
    protocol.send_fremen(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->speed,
        this->direction,
        this->moving,
        this->selected,
        this->attacking,
        this->enemy_position.x,
        this->enemy_position.y,
        this->waiting,
        client_socket); 
}


void Fremen::occupy(Board & board){
    board.occupy(this->position.x,this->position.y,this->ID);
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
            board.occupy(position.x,position.y,this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}

Infantry::Infantry(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
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
    this->enemy_position.x = -1;
    this->enemy_position.y = -1;
}
void Infantry::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;
    this->attacking = false;
    this->targeting =false;
    this->pending_move.push(Position(x, y));        
}

void Infantry::attack(int x, int y, Board& board){
    this->targeting = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;
    //  Get the surrounding positions of my enemy
    int enemy_dim_x = (board.getElementAt(x,y))->getDimX();
    int enemy_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(this->enemy_position, enemy_dim_x, enemy_dim_y); 
    //  Find the first position that is neither occupied nor reserved
    for (Position pos : attacking_positions){
        if(board.canTraverse(pos.x, pos.y)  && (board.getReserveID(pos.x,pos.y) == -1 || board.getReserveID(pos.x,pos.y) == this->ID) && !board.isOccupied(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        } 
    } 
    //std::cout << "Moving to the position: " << this->moving_position << std::endl;
    //  Move there
    this->pending_move.push(Position(this->moving_position.x,this->moving_position.y));
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
    Selectable::update(state,board);
    
    if(board.canSlowDown(this->position.x,this->position.y)){
        this->speed = INFANTRY_SPEED/2;
    } else{
        this->speed = INFANTRY_SPEED;
    }

    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
    //  Si no me estoy moviendo ni atacando, buscar enemigos
    if(this->moving == false && this->targeting == false){
		if (this->enemySearch(board) == true){
            //std::cout << "Attacking position: "<< this->enemy_position << std::endl;
            this->attack(this->enemy_position.x,this->enemy_position.y,board);
        }
    }
    //  Si estoy atacando, perseguir
    if (this->targeting == true){
        //  If there is still an enemy at the position
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            //  If he is on range, stop & shoot
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                this->focus(this->enemy_position);
                this->waiting = true;    
                this->attacking = true;
                this->moving = false;
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            //  If the enemy left the position. Stop (this will get the unit on searchEnemy() mode again)
            this->waiting = false;
            this->moving = false;
            this->targeting = false;
            this->attacking = false;
            this->enemy_position.x = -1;
            this->enemy_position.y = -1;	
            this->current_time = 0;
            return;
        }
    }
    //  Si me estoy moviendo
    if(this->moving == true){
        //  Si el camino termino, dejar de moverse
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
            return;
        }
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        Position next = remaining_path.back();
        this->focus(next);
        //  Miro la próxima posición
        if(board.canTraverse(next.x, next.y)  && (board.getReserveID(next.x,next.y) == -1 || board.getReserveID(next.x,next.y) == this->ID) && !board.isOccupied(next.x,next.y)){            //  Si puedo ir, ocuparla
            this->waiting = false;
            board.reserve(next.x,next.y,this->ID);
            this->next_position = next;          
            this->current_time+=this->speed;    //  Increase counter
        } else {
            //  Si no puedo ir 
            if (this->remaining_path.size() <= 1) {
                //  Me quedo
                this->waiting = true;
                this->moving = false;
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                //  Sino, recalculo el camino a destino
                this->waiting = true;
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                Position next_position = remaining_path.back();
                this->focus(next_position);
            }   
        }
        if (this->current_time >= this->movement_time) {
            //  Cuando se cumple el tiempo, cambiar de posición
            this->current_time = 0; //  Reset counter
            board.disoccupy(this->position.x,this->position.y);
            board.unReserve(this->next_position.x,this->next_position.y);
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
}

void Infantry::sendState(Protocol & protocol, Socket & client_socket){
    //std::cout << "Now this direction: " << this->direction << " Will be sent" << std::endl;
    protocol.send_infantry(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->speed,
        this->direction,
        this->moving,
        this->selected,
        this->attacking,
        this->enemy_position.x,
        this->enemy_position.y,
        this->waiting,
        client_socket); 
}


void Infantry::occupy(Board & board){
    board.occupy(this->position.x,this->position.y,this->ID);
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
            board.occupy(position.x,position.y,this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}

Tank::Tank(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
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
    this->enemy_position.x = -1;
    this->enemy_position.y = -1;
}
void Tank::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;
    this->attacking = false;
    this->targeting =false;    
    this->pending_move.push(Position(x, y));        
}

void Tank::attack(int x, int y, Board& board){
    this->targeting = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;
    //  Get the surrounding positions of my enemy
    int enemy_dim_x = (board.getElementAt(x,y))->getDimX();
    int enemy_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(this->enemy_position, enemy_dim_x, enemy_dim_y); 
    //  Find the first position that is neither occupied nor reserved
    for (Position pos : attacking_positions){
        if(board.canTraverse(pos.x, pos.y)  && (board.getReserveID(pos.x,pos.y) == -1 || board.getReserveID(pos.x,pos.y) == this->ID) && !board.isOccupied(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        } 
    } 
    //std::cout << "Moving to the position: " << this->moving_position << std::endl;
    //  Move there
    this->pending_move.push(Position(this->moving_position.x,this->moving_position.y));
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
    Selectable::update(state,board);
    
    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
    //  Si no me estoy moviendo ni atacando, buscar enemigos
    if(this->moving == false && this->targeting == false){
		if (this->enemySearch(board) == true){
            //std::cout << "Attacking position: "<< this->enemy_position << std::endl;
            this->attack(this->enemy_position.x,this->enemy_position.y,board);
        }
    }
    //  Si estoy atacando, perseguir
    if (this->targeting == true){
        //  If there is still an enemy at the position
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            //  If he is on range, stop & shoot
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                this->focus(this->enemy_position);
                this->waiting = true;    
                this->attacking = true;
                this->moving = false;
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            //  If the enemy left the position. Stop (this will get the unit on searchEnemy() mode again)
            this->waiting = false;
            this->moving = false;
            this->targeting = false;
            this->attacking = false;
            this->enemy_position.x = -1;
            this->enemy_position.y = -1;	
            this->current_time = 0;
            return;
        }
    }
    //  Si me estoy moviendo
    if(this->moving == true){
        //  Si el camino termino, dejar de moverse
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
            return;
        }
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        Position next = remaining_path.back();
        this->focus(next);
        //  Miro la próxima posición
        if(board.canTraverse(next.x, next.y)  && (board.getReserveID(next.x,next.y) == -1 || board.getReserveID(next.x,next.y) == this->ID) && !board.isOccupied(next.x,next.y)){            //  Si puedo ir, ocuparla
            this->waiting = false;
            board.reserve(next.x,next.y,this->ID);
            this->next_position = next;          
            this->current_time+=this->speed;    //  Increase counter
        } else {
            //  Si no puedo ir 
            if (this->remaining_path.size() <= 1) {
                //  Me quedo
                this->waiting = true;
                this->moving = false;
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                //  Sino, recalculo el camino a destino
                this->waiting = true;
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                Position next_position = remaining_path.back();
                this->focus(next_position);
            }   
        }
        if (this->current_time >= this->movement_time) {
            //  Cuando se cumple el tiempo, cambiar de posición
            this->current_time = 0; //  Reset counter
            board.disoccupy(this->position.x,this->position.y);
            board.unReserve(this->next_position.x,this->next_position.y);
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
}

void Tank::sendState(Protocol & protocol, Socket & client_socket){
    //std::cout << "Now this direction: " << this->direction << " Will be sent" << std::endl;
    protocol.send_tank(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->speed,
        this->direction,
        this->moving,
        this->selected,
        this->attacking,
        this->enemy_position.x,
        this->enemy_position.y,
        this->waiting,
        client_socket); 
}


void Tank::occupy(Board & board){
    board.occupy(this->position.x,this->position.y,this->ID);
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
            board.occupy(position.x,position.y,this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}

Devastator::Devastator(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
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
    this->enemy_position.x = -1;
    this->enemy_position.y = -1;
}
void Devastator::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;
    this->attacking = false;
    this->targeting =false;
    this->pending_move.push(Position(x, y));        
}

void Devastator::attack(int x, int y, Board& board){
    this->targeting = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;
    //  Get the surrounding positions of my enemy
    int enemy_dim_x = (board.getElementAt(x,y))->getDimX();
    int enemy_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(this->enemy_position, enemy_dim_x, enemy_dim_y); 
    //  Find the first position that is neither occupied nor reserved
    for (Position pos : attacking_positions){
        if(board.canTraverse(pos.x, pos.y)  && (board.getReserveID(pos.x,pos.y) == -1 || board.getReserveID(pos.x,pos.y) == this->ID) && !board.isOccupied(position.x,position.y)){
	        this->moving_position = pos;
            break;
        } 
    } 
    //std::cout << "Moving to the position: " << this->moving_position << std::endl;
    //  Move there
    this->pending_move.push(Position(this->moving_position.x,this->moving_position.y));
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
    Selectable::update(state,board);
    
    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
    //  Si no me estoy moviendo ni atacando, buscar enemigos
    if(this->moving == false && this->targeting == false){
		if (this->enemySearch(board) == true){
            //std::cout << "Attacking position: "<< this->enemy_position << std::endl;
            this->attack(this->enemy_position.x,this->enemy_position.y,board);
        }
    }
    //  Si estoy atacando, perseguir
    if (this->targeting == true){
        //  If there is still an enemy at the position
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            //  If he is on range, stop & shoot
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                this->focus(this->enemy_position);
                this->waiting = true;    
                this->attacking = true;
                this->moving = false;
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            //  If the enemy left the position. Stop (this will get the unit on searchEnemy() mode again)
            this->waiting = false;
            this->moving = false;
            this->targeting = false;
            this->attacking = false;
            this->enemy_position.x = -1;
            this->enemy_position.y = -1;	
            this->current_time = 0;
            return;
        }
    }
    //  Si me estoy moviendo
    if(this->moving == true){
        //  Si el camino termino, dejar de moverse
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
            return;
        }
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        Position next = remaining_path.back();
        this->focus(next);
        //  Miro la próxima posición
        if(board.canTraverse(next.x, next.y)  && (board.getReserveID(next.x,next.y) == -1 || board.getReserveID(next.x,next.y) == this->ID) && !board.isOccupied(next.x,next.y)){            //  Si puedo ir, ocuparla
            this->waiting = false;
            board.reserve(next.x,next.y,this->ID);
            this->next_position = next;          
            this->current_time+=this->speed;    //  Increase counter
        } else {
            //  Si no puedo ir 
            if (this->remaining_path.size() <= 1) {
                //  Me quedo
                this->waiting = true;
                this->moving = false;
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                //  Sino, recalculo el camino a destino
                this->waiting = true;
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                Position next_position = remaining_path.back();
                this->focus(next_position);
            }   
        }
        if (this->current_time >= this->movement_time) {
            //  Cuando se cumple el tiempo, cambiar de posición
            this->current_time = 0; //  Reset counter
            board.disoccupy(this->position.x,this->position.y);
            board.unReserve(this->next_position.x,this->next_position.y);
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
}

void Devastator::sendState(Protocol & protocol, Socket & client_socket){
    //std::cout << "Now this direction: " << this->direction << " Will be sent" << std::endl;
    protocol.send_devastator(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->speed,
        this->direction,
        this->moving,
        this->selected,
        this->attacking,
        this->enemy_position.x,
        this->enemy_position.y,
        this->waiting,
        client_socket);  
}


void Devastator::occupy(Board & board){
    board.occupy(this->position.x,this->position.y,this->ID);
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
            board.occupy(position.x,position.y,this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}

Sardaukar::Sardaukar(int ID,player_t faction, int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack,int range)
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
    this->enemy_position.x = -1;
    this->enemy_position.y = -1;
}
void Sardaukar::react(int x, int y, Board& board) {
    if (board.hasEnemy(x,y,this->faction)){
        this->attack(x,y,board);
        return;
    }
    if (!board.canTraverse(x,y))        
        return;
    this->attacking = false;
    this->targeting =false;
    this->pending_move.push(Position(x, y));        
}

void Sardaukar::attack(int x, int y, Board& board){
    this->targeting = true;
	this->enemy_position = Position(x,y);
    this->moving_position = this->enemy_position;
    //  Get the surrounding positions of my enemy
    int enemy_dim_x = (board.getElementAt(x,y))->getDimX();
    int enemy_dim_y = (board.getElementAt(x,y))->getDimY();
    std::vector<Position> attacking_positions = board.getSurroundings(this->enemy_position, enemy_dim_x, enemy_dim_y); 
    //  Find the first position that is neither occupied nor reserved
    for (Position pos : attacking_positions){
        if(board.canTraverse(pos.x, pos.y)  && (board.getReserveID(pos.x,pos.y) == -1 || board.getReserveID(pos.x,pos.y) == this->ID) && !board.isOccupied(pos.x,pos.y)){
	        this->moving_position = pos;
            break;
        } 
    } 
    //std::cout << "Moving to the position: " << this->moving_position << std::endl;
    //  Move there
    this->pending_move.push(Position(this->moving_position.x,this->moving_position.y));
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
    Selectable::update(state,board);
    
    if(board.canSlowDown(this->position.x,this->position.y)){
        this->speed = SARDAUKAR_SPEED/2;
    } else{
        this->speed = SARDAUKAR_SPEED;
    }

    if (this->pending_move.size() != 0) {
        if (this->current_time == 0) {
            Position move_to = this->pending_move.front();
            this->pending_move.pop();
            this->move(move_to.x,move_to.y,board);
        }
    }
    //  Si no me estoy moviendo ni atacando, buscar enemigos
    if(this->moving == false && this->targeting == false){
		if (this->enemySearch(board) == true){
            //std::cout << "Attacking position: "<< this->enemy_position << std::endl;
            this->attack(this->enemy_position.x,this->enemy_position.y,board);
        }
    }
    //  Si estoy atacando, perseguir
    if (this->targeting == true){
        //  If there is still an enemy at the position
        if(board.hasEnemy(this->enemy_position.x,this->enemy_position.y,this->faction)){
            //  If he is on range, stop & shoot
            if(board.get_distance_between(this->position,this->enemy_position) < this->range){
                this->focus(this->enemy_position);
                this->waiting = true;    
                this->attacking = true;
                this->moving = false;
                board.dealDamage(this->enemy_position.x,this->enemy_position.y,this->attack_points);
            }
        } else{
            //  If the enemy left the position. Stop (this will get the unit on searchEnemy() mode again)
            this->waiting = false;
            this->moving = false;
            this->targeting = false;
            this->attacking = false;
            this->enemy_position.x = -1;
            this->enemy_position.y = -1;	
            this->current_time = 0;
            return;
        }
    }
    //  Si me estoy moviendo
    if(this->moving == true){
        //  Si el camino termino, dejar de moverse
        if (this->remaining_path.size() == 0) {
            this->moving = false;
            this->current_time = 0;
            return;
        }
        //  Si se esta moviendo, siempre apunta a la direccion en la que se mueve
        Position next = remaining_path.back();
        this->focus(next);
        //  Miro la próxima posición
        if(board.canTraverse(next.x,next.y) && (board.getReserveID(next.x,next.y) == -1 || board.getReserveID(next.x,next.y) == this->ID) && !board.isOccupied(next.x,next.y)){            //  Si puedo ir, ocuparla
            this->waiting = false;
            board.reserve(next.x,next.y,this->ID);
            this->next_position = next;          
            this->current_time+=this->speed;    //  Increase counter
        } else {
            //  Si no puedo ir 
            if (this->remaining_path.size() <= 1) {
                //  Me quedo
                this->waiting = true;
                this->moving = false;
                std::vector<Position> empty_path;
                this->remaining_path = empty_path;
            } else {
                //  Sino, recalculo el camino a destino
                this->waiting = true;
                this->move(this->remaining_path.front().x,this->remaining_path.front().y,board);
                Position next_position = remaining_path.back();
                this->focus(next_position);
            }   
        }
        if (this->current_time >= this->movement_time) {
            //  Cuando se cumple el tiempo, cambiar de posición
            this->current_time = 0; //  Reset counter
            board.disoccupy(this->position.x,this->position.y);
            board.unReserve(this->next_position.x,this->next_position.y);
            this->position = this->remaining_path.back();
            this->occupy(board);
            this->remaining_path.pop_back();
        }
    }
}

void Sardaukar::sendState(Protocol & protocol, Socket & client_socket){
    //std::cout << "Now this direction: " << this->direction << " Will be sent" << std::endl;
    protocol.send_sardaukar(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->speed,
        this->direction,
        this->moving,
        this->selected,
        this->attacking,
        this->enemy_position.x,
        this->enemy_position.y,
        this->waiting,
        client_socket); 
}


void Sardaukar::occupy(Board & board){
    board.occupy(this->position.x,this->position.y,this->ID);
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
            board.occupy(position.x,position.y,this->ID);
            *spice -= this->spice;
            return RES_CREATE_UNIT_SUCCESS;
        }
    }
    return RES_CREATE_UNIT_FAILURE_SPACE;
}
