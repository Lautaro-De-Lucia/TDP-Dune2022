#include "building.h"

Building::Building(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y)
:
Selectable(ID,faction,LP,pos,dim_x,dim_y,false)
{
    this->spice = spice;
    this->energy = energy;
    std::cout << "New building created of faction: " << this->faction << std::endl;
}

bool Building::place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity){
    
    if ((spice - this->getSpice()) < 0){
        std::cerr << "Not enough Spice!!!" << std::endl;
        return false;
    }
    if ((energy - this->getEnergy()) < 0){
        std::cerr << "Not enough Energy!!!" << std::endl;
        return false;
    }
    if (board.canPlace(Position(pos_x,pos_y),this->getDimX(),this->getDimY()) == FAILED_TO_ADD){
        std::cerr << "Can't build in this position!!!" << std::endl;
        return false;
    }
    this->setPosition(Position(pos_x,pos_y));
    return true;
}

int Building::getSpice(){
    return this->spice;
}
int Building::getEnergy(){
    return this->energy;
}

AirTrap::AirTrap(int ID,player_t faction, int LP,int spice,int energy, Position pos, int dim_x,int dim_y, int c_energy)
:
Building(ID,faction, LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Wind Trap";
    this->c_energy = c_energy;
}

bool AirTrap::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    
    spice -= this->spice;
    energy += this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return true;
}

int AirTrap::getCEnergy() {
    return this->c_energy;
}

Barrack::Barrack(int ID,player_t faction,int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Barrack";
}

bool Barrack::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return true;
}

void Building::react(int x, int y, Board & board){
    for (size_t j = 0 ; j < this->getDimY() ; j++)
        for (size_t i = 0 ; i < this->getDimX() ; i++)
            if( x == this->position.x+i && y == this->position.y+j){        
                board.makeCreator(this->ID);
                return;
            }
}

Refinery::Refinery(int ID,player_t faction, int LP,int spice,int energy, Position pos, int dim_x,int dim_y, int c_spice)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Refinery";
    this->c_spice= c_spice;
}

bool Refinery::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    if (!Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity))
        return false;
    spice -= this->spice;
    energy -= this->energy;
    spice_capacity += this->c_spice; 

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            std::cout<< "Marking position (" << this->position.x+i << "," <<this->position.y+j<<") as occupied"<< std::endl;
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }
    std::vector<Position> deposit_positions = board.getSurroundings(this->position, this->getDimX(), this->getDimY());
    board.addDepositPositions(deposit_positions);

    return true;
}

int Refinery::getCSpice() {
    return this->c_spice;
}
