#include "server_gameresources.h"

extern int ID;

GameResources::GameResources(std::vector<std::vector<cell_t>> cells) 
: 
board(cells,this->elements) 
{}

Cell & GameResources::getCell(int x, int y){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getCell(x,y);
}

void GameResources::addSandPosition(int x, int y){
    std::lock_guard<std::mutex> locker(this->lock);
    this->board.addSandPosition(x,y);
}
status_t GameResources::canPlace(const Position& location, int dim_x,int dim_y) {
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.canPlace(location,dim_x,dim_y);
}

bool GameResources::canDeposit(int x, int y,player_t faction){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.canDeposit(x,y,faction);
}

bool GameResources::canHarvest(int x, int y){return this->board.canHarvest(x,y);}
bool GameResources::canTraverse(int x, int y){return this->board.canTraverse(x,y);}

bool GameResources::hasEnemy(int x, int y, player_t player_faction){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.hasEnemy(x,y,player_faction);
}

void GameResources::dealDamage(int x, int y, int damage){
    std::lock_guard<std::mutex> locker(this->lock);
    this->board.dealDamage(x,y,damage);
}

std::unique_ptr<Selectable> & GameResources::getElementAt(int x, int y){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getElementAt(x,y);
}

int GameResources::getTotalCreators(player_t faction, unit_t type){
    return this->board.getTotalCreators(faction,type);
}


int GameResources::getBoardWidth() {
    return this->board.get_width();  
}
int GameResources::getBoardHeight() {
    return this->board.get_height();
}

std::vector<Position> GameResources::getTraversableNeighborsOf(Position pos,size_t distance){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.get_traversable_neighbors_of(pos,distance); 
}

size_t GameResources::getDistanceBetween(Position pos1, Position pos2) {
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.get_distance_between(pos1,pos2);
}

void GameResources::addDepositPositions(std::vector<Position> & new_deposit_positions){
    std::lock_guard<std::mutex> locker(this->lock);
    this->board.addDepositPositions(new_deposit_positions);
}

std::vector<Position> & GameResources::getDepositPositions(){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getDepositPositions();
}

int GameResources::getCreator(player_t faction,unit_t type){
    return this->board.getCreator(faction,type);
}

//  void GameResources::makeCreator(int building_ID){this->board.makeCreator(building_ID);}
//  void GameResources::removeCreator(unit_t unit) {this->board.removeCreator(unit);}


std::vector<Position> GameResources::getSurroundings(Position position, int e_dim_x, int e_dim_y){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getSurroundings(position,e_dim_x,e_dim_y);
}

response_t GameResources::createUnit(player_t faction,unit_t type,int & spice){    
    std::lock_guard<std::mutex> locker(this->lock);
    if (this->board.getCreator(faction,type) == -1)
        return RES_CREATE_UNIT_FAILURE_CREATOR;

    Position building_pos = (elements.at(board.getCreator(faction,type)))->getPosition();
    int building_dim_x = (elements.at(board.getCreator(faction,type)))->getDimX();
    int building_dim_y = (elements.at(board.getCreator(faction,type)))->getDimY();

    std::vector<Position> positions = board.getSurroundings(building_pos, building_dim_x, building_dim_y); //  FAILING HERE

    std::unique_ptr<Unit> unit = UnitFactory::create(type,faction,ID);
    //  Attempt adding it
    response_t res = (*unit).place(board,positions,&spice);
    if( res == RES_CREATE_UNIT_SUCCESS ){
        (this->elements).insert({ID, std::move(unit)});
        ID++;
    }
    return res;
}

response_t GameResources::createBuilding(player_t faction,building_t type,int pos_x,int pos_y,int & spice,int & c_spice,int & energy,int & c_energy){
    std::lock_guard<std::mutex> locker(this->lock);
    std::unique_ptr<Building> building = BuildingFactory::manufacture(type,faction,ID);
    //  Attempt to add to board
    std::cout << "Creando un edificio de faccion" << building->getFaction() << std::endl;
    response_t res = (*building).place(board,pos_x,pos_y,spice,c_spice,energy,c_energy);
    if (res == RES_CREATE_BUILDING_SUCCESS){
        (this->elements).insert({ID, std::move(building)});
        ID++;
        this->board.addUnitCreator(faction,type);
    }
    return res;
}

void GameResources::selectElement(player_t faction,int pos_x, int pos_y){
    std::lock_guard<std::mutex> locker(this->lock);
    for (auto& e : this->elements){
        if(e.second->getFaction() == faction){
            if (e.second->contains(Position(pos_x,pos_y)))
                e.second->select();
            else
                e.second->unselect();
        }
    }
}

void GameResources::selectElements(player_t faction,int Xmin, int Xmax,int Ymin, int Ymax){
    std::lock_guard<std::mutex> locker(this->lock);
    Area selection(Xmin,Xmax,Ymin,Ymax);
    for (auto& e : this->elements){
        if(e.second->getFaction() == faction){
            e.second->unselect();
            if (e.second->isWithin(selection))
                e.second->select();
        }
    }
}
void GameResources::reactToPosition(player_t faction,int pos_x,int pos_y){
    //  Traverse elements and make each selected unit handle the cell
    for (auto& e : this->elements){
        if(e.second->getFaction() == faction){
            if (e.second->isSelected()){
                e.second->react(pos_x,pos_y,this->board);
            }
        }
    }
}

int GameResources::totalElements(){
    return this->elements.size();
}

void GameResources::sendElements(Protocol & protocol, Socket & client_socket){
    for (auto& e : this->elements)
        e.second->sendState(protocol,client_socket);
}

int GameResources::getTotalChangedCells(){
    return this->board.getChangedSandPositions().size();
}
void GameResources::clearChangedCells(){
    this->board.clearSandPositions();
}
std::vector<Position> GameResources::getChangedCells(){
    return this->board.getChangedSandPositions();
}

void GameResources::update(){
    State state;
    std::vector<State> states;
    for (auto& e : this->elements){
        state.ID = e.first;
        e.second->update(state,this->board);
        states.push_back(state);
    }
}
