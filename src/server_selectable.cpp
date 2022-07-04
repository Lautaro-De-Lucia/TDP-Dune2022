#include "server_selectable.h"

Selectable::Selectable(int id,player_t faction,int LP, Position position, int dim_x, int dim_y, bool moves){
    this->ID = id;
    this->faction = faction;
    this->LP = LP;
    this->position = position;
    this->dim_x = dim_x;
    this->dim_y = dim_y;
    this->selected = false;
    this->is_movable = moves;
    std::vector<Position> empty_path;
    this->remaining_path = empty_path;
}

bool Selectable::canCreate(unit_t type){return false;}
bool Selectable::canEnable(unit_t type){return false;}
bool Selectable::canStoreSpice(int & spice){return false;}
bool Selectable::canCostTheGame(){return false;}
bool Selectable::isSelected(){return this->selected;}
bool Selectable::moves() {return this->is_movable;}
void Selectable::select(){this->selected = true;}
void Selectable::unselect(){this->selected = false;}

bool Selectable::contains(const Position& pos){
    if ((pos.x == (this->position.x)) && (pos.y == (this->position.y)))
        return true;
    for (size_t i = 0 ; i < this->dim_x ; i++) 
        for (size_t j = 0 ; j < this->dim_y ; j++) 
            if ((pos.x == ((this->position.x)+i)) && (pos.y == ((this->position.y)+j)))
                return true;
    return false;
}

void Selectable::react(int x, int y, Board& board){
    std::cerr << "I really shouldn't be here" << std::endl;
}

void Selectable::receiveDamage(int damage){
    this->LP = this->LP-damage;
}

void Selectable::update(State& state, Board& board){
    state.LP = this->LP;
    state.position = this->position;
    state.selected = this->selected;
}


bool Selectable::isWithin(const Area& selection){
    for (size_t i = 0 ; i < this->dim_x ; i++) {
        for (size_t j = 0 ; j < this->dim_y ; j++) {           
            if (
            (this->position.x+i<selection.Xmax && this->position.x+i>selection.Xmin)
            &&
            (this->position.y+j<selection.Ymax && this->position.y+j>selection.Ymin)
            ){
            return true;
            }
        }
    }
    return false;
}

std::string Selectable::getName(){return this->name;}
Position Selectable::getPosition(){return this->position;}
player_t Selectable::getFaction(){return this->faction;}
int Selectable::getEnergy(){return 0;}
int Selectable::getID(){return this->ID;}
int Selectable::getLP(){return this->LP;}
int Selectable::getDimX(){return this->dim_x;}
int Selectable::getDimY(){return this->dim_y;}
std::vector<Position>& Selectable::get_remaining_path() {return this->remaining_path;}
std::vector<Position> Selectable::getPositions(){
    std::vector<Position> positions;
    for (size_t i = 0; i < this->dim_x ; i++)
        for(size_t j = 0; j < this->dim_y ; j++)
            positions.push_back(Position(this->position.x+i,this->position.y+j));
    return positions;
}
void Selectable::setPosition(Position position){this->position = position;}
void Selectable::sendState(Protocol & protocol,Socket & client_socket){
    std::cerr << "I really shouldn't be here" << std::endl;
}



