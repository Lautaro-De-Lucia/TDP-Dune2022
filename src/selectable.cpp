#include "selectable.h"

Area::Area(uint16_t Xmin,uint16_t Xmax, uint16_t Ymin, uint16_t Ymax){
    this->Xmin = Xmin;
    this->Xmax = Xmax;
    this->Ymin = Ymin;
    this->Ymax = Ymax;
}

Selectable::Selectable(uint16_t LP, Position pos, uint8_t dim_x, uint8_t dim_y){
    this->LP = LP;
    this->position = pos;
    this->dim_x = dim_x;
    this->dim_y = dim_y;
    this->selected = false;
}

bool Selectable::isSelected(){
    return this->selected;
}

void Selectable::select(){
    std::cout << "Unit selected!" << std::endl;
    this->selected = true;
}

void Selectable::unselect(){
    std::cout << "Unit unselected!" << std::endl;
    this->selected = false;
}

bool Selectable::contains(const Position & position){
    for (size_t j = 0 ; j < this->dim_y ; j++) {
        for (size_t i = 0 ; i < this->dim_x ; i++) {
            if ((position.x == ((this->position.x)+i)) && (position.y == ((this->position.y)+j)))
            return true;
        }
    }
    return false;
}

void Selectable::react(Cell & location){
    std::cout << "reacting to cell in location: " << location.getPosition() << std::endl;
}

bool Selectable::isWithin(const Area & selection){
    for (size_t j = 0 ; j < this->dim_y ; j++) {
        for (size_t i = 0 ; i < this->dim_x ; i++) {           
            if(
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
    for (size_t j = 0 ; j < this->dim_y ; j++) {
        for (size_t i = 0 ; i < this->dim_x ; i++) {
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
                if( i == 0)
                    surroundings.push_back(Position(this->position.x+i-1,this->position.y+j));
                if( i == this->dim_x-1 ) 
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

void Selectable::setPosition(Position position){
    this->position = position;
}
uint8_t Selectable::getDimX(){
    return this->dim_x;
}
uint8_t Selectable::getDimY(){
    return this->dim_y;
}

void Selectable::getState(State & state){
    state.LP = this->LP;
    state.position = this->position;
    state.selected = this->selected;
}