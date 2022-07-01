#include "server_building.h"

Building::Building(int ID,player_t faction, int LP, int spice, int energy, Position pos, int dim_x, int dim_y)
:
Selectable(ID,faction,LP,pos,dim_x,dim_y,false)
{
    this->spice = spice;
    this->energy = energy;
}

bool Building::canCreate(unit_t type){return false;}
bool Building::canEnable(unit_t type){return false;}
bool Building::canCostTheGame(){return false;}


response_t Building::place(Board& board, int pos_x, int pos_y, int& spice,int& energy){    
    if ((spice - this->spice) < 0)
        return RES_CREATE_BUILDING_FAILURE_SPICE;
    if ((energy - this->energy) < 0)
        return RES_CREATE_BUILDING_FAILURE_ENERGY;
    if (board.canPlace(Position(pos_x,pos_y),this->dim_x,this->dim_y) == FAILED_TO_ADD)
        return RES_CREATE_BUILDING_FAILURE_TERRAIN;
    this->setPosition(Position(pos_x,pos_y));
    return RES_CREATE_BUILDING_SUCCESS;
}

int Building::getSpice(){
    return this->spice;
}
int Building::getEnergy(){
    return this->energy;
}

std::vector<Position> Building::getPositions(){
    std::vector<Position> positions;
    for (size_t i = 0; i < this->dim_x ; i++)
        for(size_t j = 0; j < this->dim_y ; j++)
            positions.push_back(Position(this->position.x+i,this->position.y+j));
    return positions;
}

void Building::sendState(Protocol & protocol,Socket & client_socket){
    std::cerr << "I shouldn't be here either" << std::endl;
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

response_t AirTrap::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;
    
    spice -= this->spice;
    energy += this->energy;

    for (size_t j = 0 ; j < this->dim_y ; j++)
        for (size_t i = 0 ; i < this->dim_x ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);    
    
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

bool Barrack::canCreate(unit_t type){return (type == FREMEN || type == INFANTRY || type == SARDAUKAR)? true : false;}

response_t Barrack::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->dim_y ; j++)
        for (size_t i = 0 ; i < this->dim_x ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
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

bool Refinery::canCreate(unit_t type){return false;}
bool Refinery::canStoreSpice(int & spice){
    spice = this->c_spice;
    return true;
}



response_t Refinery::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;
    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->dim_y ; j++)
        for (size_t i = 0 ; i < this->dim_x ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);
        
    std::vector<Position> deposit_positions = board.getSurroundings(this->position, this->dim_x, this->dim_y);    
    board.addDepositPositions(this->faction,deposit_positions);

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

bool LightFactory::canCreate(unit_t type){return (type == TRIKE || type == HARVESTER)? true : false;}


response_t LightFactory::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->dim_y ; j++)
        for (size_t i = 0 ; i < this->dim_x ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);

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

bool HeavyFactory::canCreate(unit_t type){return (type == TRIKE || type == DEVASTATOR)? true : false;}

response_t HeavyFactory::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->dim_y ; j++)
        for (size_t i = 0 ; i < this->dim_x ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);

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

bool Silo::canStoreSpice(int & spice){
    spice = this->c_spice;
    return true;
}

response_t Silo::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->dim_y ; j++)
        for (size_t i = 0 ; i < this->dim_x ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);

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

response_t Palace::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->dim_y ; j++) 
        for (size_t i = 0 ; i < this->dim_x ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);

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

bool Palace::canEnable(unit_t type){return (type == SARDAUKAR || type == DEVASTATOR) ? true : false;}

ConstructionYard::ConstructionYard(int ID,player_t faction,int LP,int spice,int energy, Position pos, int dim_x,int dim_y)
:
Building(ID,faction,LP,spice,energy,pos,dim_x,dim_y)
{
    this->name = "Construction Yard";
}

bool ConstructionYard::canCostTheGame(){
    return true;
}

response_t ConstructionYard::place(Board& board,int pos_x,int pos_y,int& spice,int& energy){
    response_t res;
    res = Building::place(board,pos_x,pos_y,spice,energy);
    if(res != RES_CREATE_BUILDING_SUCCESS)
        return res;

    spice -= this->spice;
    energy -= this->energy;

    for (size_t j = 0 ; j < this->getDimY() ; j++)
        for (size_t i = 0 ; i < this->getDimX() ; i++)
            board.getCell(this->position.x+i,this->position.y+j).occupy(this->ID);

    return RES_CREATE_BUILDING_SUCCESS;
}

void ConstructionYard::sendState(Protocol & protocol,Socket & client_socket){
    protocol.send_construction_yard(
        this->ID,
        this->faction,
        this->LP,
        this->position.x,
        this->position.y,
        this->selected,
        client_socket
    );
}


