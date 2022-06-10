#include "selectable.h"

Area::Area(int Xmin,int Xmax, int Ymin, int Ymax){
    this->Xmin = Xmin;
    this->Xmax = Xmax;
    this->Ymin = Ymin;
    this->Ymax = Ymax;
}

Selectable::Selectable(player_t faction,int LP, Position position, int dim_x, int dim_y, bool moves){
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

bool Selectable::isSelected(){
    return this->selected;
}

bool Selectable::moves() {
    return this->is_movable;
}

void Selectable::select(){
    std::cout << "Unit selected!" << std::endl;
    this->selected = true;
}

void Selectable::unselect(){
    std::cout << "Unit unselected!" << std::endl;
    this->selected = false;
}

bool Selectable::contains(const Position& pos){

    if ((pos.x == (this->position.x)) && (pos.y == (this->position.y)))
        return true;

    for (size_t i = 0 ; i < this->dim_x ; i++) {
        for (size_t j = 0 ; j < this->dim_y ; j++) {
            if ((pos.x == ((this->position.x)+i)) && (pos.y == ((this->position.y)+j)))
                return true;
        }
    }
    return false;
}

void Selectable::react(int x, int y, Board & board){
    std::cout << "reacting to cell in location: (" << x << "," << y << ") [selectable]" << std::endl;
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

std::vector<Position> Selectable::getSurroundings(){
    std::vector <Position> surroundings;
    for (size_t i = 0 ; i < this->dim_x ; i++) {
        for (size_t j = 0 ; j < this->dim_y ; j++) {
            if ( j == 0 ){
                if (i == 0){        
                    surroundings.push_back(Position(this->position.x+i-1,this->position.y+j-1));
                    surroundings.push_back(Position(this->position.x+i-1,this->position.y+j));
                }
                if (i < this->dim_x)    
                    surroundings.push_back(Position(this->position.x+i,this->position.y+j-1));
                if (i == this->dim_x-1){    
                    surroundings.push_back(Position(this->position.x+i+1,this->position.y+j-1));
                    surroundings.push_back(Position(this->position.x+i+1,this->position.y+j));
                }
            } 
            if ( j > 0 && j < this->dim_y-1 ){
                if ( i == 0)
                    surroundings.push_back(Position(this->position.x+i-1,this->position.y+j));
                if ( i == this->dim_x-1 ) 
                    surroundings.push_back(Position(this->position.x+i-1,this->position.y+j));
            }
            if ( j == this->dim_y-1){
                if (i == 0){        
                    surroundings.push_back(Position(this->position.x+i-1,this->position.y+j));
                    surroundings.push_back(Position(this->position.x+i-1,this->position.y+j+1));
                }
                if (i < this->dim_x)    
                    surroundings.push_back(Position(this->position.x+i,this->position.y+j+1));
                if (i == this->dim_x-1){    
                    surroundings.push_back(Position(this->position.x+i+1,this->position.y+j+1));
                    surroundings.push_back(Position(this->position.x+i+1,this->position.y+j));
                }
            }    
        }
    }
    return surroundings;
}

std::string Selectable::getName(){
    return this->name;
}

Position Selectable::getPosition(){
    return this->position;
}

player_t Selectable::getFaction(){
    return this->faction;
}

void Selectable::setPosition(Position position){
    this->position = position;
}
int Selectable::getDimX(){
    return this->dim_x;
}
int Selectable::getDimY(){
    return this->dim_y;
}

std::vector<Position>& Selectable::get_remaining_path() {
    return this->remaining_path;
}

void Selectable::getState(State& state){
    state.LP = this->LP;
    state.position = this->position;
    state.selected = this->selected;
}