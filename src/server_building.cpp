#include "server_building.h"

Building::Building(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y)
:
Selectable(ID,faction,LP,pos,dim_x,dim_y,false)
{
    this->spice = spice;
    this->energy = energy;
    std::cout << "New building created of faction: " << this->faction << std::endl;
}

response_t Building::place(Board& board, int pos_x, int pos_y, int& spice, int& spice_capacity, int& energy, int& energy_capacity){
    
    if ((spice - this->getSpice()) < 0){
        std::cerr << "Not enough Spice!!!" << std::endl;
        return RES_CREATE_BUILDING_FAILURE_SPICE;
    }
    if ((energy - this->getEnergy()) < 0){
        std::cerr << "Not enough Energy!!!" << std::endl;
        return RES_CREATE_BUILDING_FAILURE_ENERGY;
    }
    if (board.canPlace(Position(pos_x,pos_y),this->getDimX(),this->getDimY()) == FAILED_TO_ADD){
        std::cerr << "Can't build in this position!!!" << std::endl;
        return RES_CREATE_BUILDING_FAILURE_TERRAIN;
    }
    this->setPosition(Position(pos_x,pos_y));
    return RES_CREATE_BUILDING_SUCCESS;
}

int Building::getSpice(){
    return this->spice;
}
int Building::getEnergy(){
    return this->energy;
}

void Building::sendState(Protocol & protocol,Socket & client_socket){
    std::cout << "I shouldn't be here either" << std::endl;
}

void Building::react(int x, int y, Board & board){
    for (size_t j = 0 ; j < this->getDimY() ; j++)
        for (size_t i = 0 ; i < this->getDimX() ; i++)
            if( x == this->position.x+i && y == this->position.y+j){        
                board.makeCreator(this->ID);
                return;
            }
}

AirTrap::AirTrap(int ID,player_t faction, int LP,int spice,int energy, Position pos, int dim_x,int dim_y, int c_energy)
:
Building(ID,faction, LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Wind Trap";
    this->c_energy = c_energy;
}

response_t AirTrap::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;
    
    spice -= this->spice;
    energy += this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return RES_CREATE_BUILDING_SUCCESS;
}

int AirTrap::getCEnergy() {
    return this->c_energy;
}

void AirTrap::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_air_trap(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}

Barrack::Barrack(int ID,player_t faction,int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Barrack";
}

response_t Barrack::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return RES_CREATE_BUILDING_SUCCESS;
}

void Barrack::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_barrack(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}

Refinery::Refinery(int ID,player_t faction, int LP,int spice,int energy, Position pos, int dim_x,int dim_y, int c_spice)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Refinery";
    this->c_spice= c_spice;
}

response_t Refinery::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;
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

    return RES_CREATE_BUILDING_SUCCESS;
}

int Refinery::getCSpice() {
    return this->c_spice;
}

void Refinery::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_refinery(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}

LightFactory::LightFactory(int ID,player_t faction,int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Light Factory";
}

response_t LightFactory::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return RES_CREATE_BUILDING_SUCCESS;
}

void LightFactory::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_light_factory(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}


HeavyFactory::HeavyFactory(int ID,player_t faction,int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Heavy Factory";
}

response_t HeavyFactory::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return RES_CREATE_BUILDING_SUCCESS;
}

void HeavyFactory::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_heavy_factory(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}

Silo::Silo(int ID,player_t faction,int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Silo";
}

response_t Silo::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return RES_CREATE_BUILDING_SUCCESS;
}

void Silo::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_silo(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}

Palace::Palace(int ID,player_t faction,int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Barrack";
}

response_t Palace::place(Board& board,int pos_x,int pos_y,int& spice,int& spice_capacity,int& energy,int& energy_capacity){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,spice_capacity,energy,energy_capacity);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++){ 
        for (size_t i = 0 ; i < this->getDimX() ; i++){
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        }
    }

    return RES_CREATE_BUILDING_SUCCESS;
}

void Palace::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_palace(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}