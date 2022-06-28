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

bool GameResources::isEnabled(player_t faction,unit_t unit){
    std::lock_guard<std::mutex> locker(this->lock);
    if(unit != SARDAUKAR && unit != DEVASTATOR)
        return true;
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction)
            if(e.second->canEnable(unit))
                return true;
    return false;
}

int GameResources::getSpiceCapacity(player_t faction){
    std::lock_guard<std::mutex> locker(this->lock);
    int total_spice = 0;
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction){
            int spice;
            if(e.second->canStoreSpice(spice) == true)
                total_spice += spice;
        }
    return total_spice;
}

bool GameResources::hasLost(player_t faction){
    std::lock_guard<std::mutex> locker(this->lock);
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction)
            if(e.second->canCostTheGame() == true)
                return false;
    return true;
}

std::unique_ptr<Selectable> & GameResources::getElementAt(int x, int y){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getElementAt(x,y);
}

int GameResources::getTotalCreators(player_t faction, unit_t type){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getTotalCreators(faction,type);
}

int GameResources::getCreator(player_t faction,unit_t type){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getCreator(faction,type);
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

response_t GameResources::createBuilding(player_t faction,building_t type,int pos_x,int pos_y,int & spice,int & energy){
    std::lock_guard<std::mutex> locker(this->lock);
    std::unique_ptr<Building> building = BuildingFactory::manufacture(type,faction,ID);
    //  Attempt to add to board
    response_t res = (*building).place(board,pos_x,pos_y,spice,energy);
    if (res == RES_CREATE_BUILDING_SUCCESS){
        (this->elements).insert({ID, std::move(building)});
        ID++;
        this->board.addUnitCreator(faction,type);
        unit_t model_unit;
        if (type == BARRACK) {
            model_unit = FREMEN;
        } else if (type == REFINERY) {
            model_unit = HARVESTER;
        } else if (type == LIGHT_FACTORY) {
            model_unit = TRIKE;
        } else if (type == HEAVY_FACTORY) {
            model_unit = TANK;
        }

        if (this->board.getTotalCreators(faction, model_unit) == 1)
            this->board.makeCreator(ID - 1);
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
    std::lock_guard<std::mutex> locker(this->lock);
    for (auto& e : this->elements){
        if(e.second->getFaction() == faction){
            if (e.second->isSelected()){
                e.second->react(pos_x,pos_y,this->board);
            }
        }
    }
}

int GameResources::totalElements(){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->elements.size();
}

void GameResources::sendElements(Protocol & protocol, Socket & client_socket){
    std::lock_guard<std::mutex> locker(this->lock);
    for (auto& e : this->elements)
        e.second->sendState(protocol,client_socket);
}

int GameResources::getTotalChangedCells(){
    std::lock_guard<std::mutex> locker(this->lock); 
    return this->board.getChangedSandPositions().size();
}
void GameResources::clearChangedCells(){
    std::lock_guard<std::mutex> locker(this->lock);
    this->board.clearSandPositions();
}
std::vector<Position> GameResources::getChangedCells(){
    std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getChangedSandPositions();
}

void GameResources::update(){
    std::lock_guard<std::mutex> locker(this->lock);
    State state;
    std::vector<State> states;
    for (auto& e : this->elements){
        state.ID = e.first;
        e.second->update(state,this->board);
        states.push_back(state);
    }
}