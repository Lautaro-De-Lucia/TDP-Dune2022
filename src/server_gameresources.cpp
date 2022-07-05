#include "server_gameresources.h"

extern int ID;

GameResources::GameResources(std::vector<std::vector<cell_t>> cells) 
: 
board(cells,this->elements) 
{}

Cell & GameResources::getCell(int x, int y){
    return this->board.getCell(x,y);
}

int GameResources::getSpiceAt(int x, int y){
    //std::lock_guard<std::mutex> locker(this->lock);
    return this->board.getCell(x,y).getSpice();
}

void GameResources::deleteElement(int ID){
    //std::lock_guard<std::mutex> locker(this->lock);
    this->board.deleteElement(ID);
}

void GameResources::sendCreationData(std::vector<creation_t> & creation_data,Protocol & protocol, Socket & client_socket){
    protocol.send_creation_data_size(creation_data.size(), client_socket);
    for (creation_t c : creation_data)
        protocol.send_creation_data(c.creator_ID,c.unit_being_created,c.current_time,c.total_time,client_socket);
    creation_data.clear();
}

void GameResources::sendCreators(player_t faction,Protocol & protocol, Socket & client_socket){
    int barrack_id = getCreator(faction, FREMEN);
    int light_factory_id = getCreator(faction, TRIKE);
    int heavy_factory_id = getCreator(faction, TANK);
    protocol.send_creators(barrack_id, light_factory_id, heavy_factory_id, client_socket);
}

bool GameResources::isEnabled(player_t faction,unit_t unit){
    if(unit != SARDAUKAR && unit != DEVASTATOR)
        return true;
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction)
            if(e.second->canEnable(unit))
                return true;
    return false;
}

int GameResources::getSpiceCapacity(player_t faction){
    int total_spice = 0;
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction){
            int spice;
            if(e.second->canStoreSpice(spice) == true)
                total_spice += spice;
        }
    return total_spice;
}

int GameResources::getTotalEnergy(player_t faction){
    int total_energy = 0;
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction){
            total_energy += e.second->getEnergy();
        }
    return total_energy;
}

bool GameResources::hasLost(player_t faction){
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction)
            if(e.second->canCostTheGame() == true)
                return false;
    return true;
}

std::unique_ptr<Selectable> & GameResources::getElementAt(int x, int y){
    return this->board.getElementAt(x,y);
}

int GameResources::getTotalCreators(player_t faction, unit_t type){
    return this->board.getTotalCreators(faction,type);
}

int GameResources::getCreator(player_t faction,unit_t type){
    return this->board.getCreator(faction,type);
}

response_t GameResources::createUnit(player_t faction,unit_t type,int & spice){    
    //std::lock_guard<std::mutex> locker(this->lock);

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
    //std::lock_guard<std::mutex> locker(this->lock);
    std::unique_ptr<Building> building = BuildingFactory::manufacture(type,faction,ID);
    //  Attempt to add to board
    response_t res = (*building).place(board,pos_x,pos_y,spice,energy);
    if (res == RES_CREATE_BUILDING_SUCCESS){
        (this->elements).insert({ID, std::move(building)});
        this->board.addUnitCreator(faction,type);
        unit_t model_unit = UNDEFINED;
        if (type == BARRACK) {
            model_unit = FREMEN;
        } else if (type == LIGHT_FACTORY) {
            model_unit = TRIKE;
        } else if (type == HEAVY_FACTORY) {
            model_unit = TANK;
        }
        if (this->board.getTotalCreators(faction, model_unit) == 1)
            this->board.makeCreator(ID);
        ID++;    
    }
    return res;
}

void GameResources::selectElement(player_t faction,int pos_x, int pos_y){
    //std::lock_guard<std::mutex> locker(this->lock);
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
    //std::lock_guard<std::mutex> locker(this->lock);
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
    //std::lock_guard<std::mutex> locker(this->lock);
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
    //std::lock_guard<std::mutex> locker(this->lock);
    this->board.clearSandPositions();
}
std::vector<Position> GameResources::getChangedCells(){
    return this->board.getChangedSandPositions();
}

void GameResources::update(){
    //std::lock_guard<std::mutex> locker(this->lock);
    for (auto& e : this->elements){
        e.second->update(this->board);
    }
}