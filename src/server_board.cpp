#include "server_board.h"


Board::Board(std::vector<std::vector<cell_t>> cell_types,std::map<int,std::unique_ptr<Selectable>> & elements) :
cells(), 
elements(elements)
{
    //  Get board dimentions
    this->dim_x = cell_types.size();
    this->dim_y = cell_types[0].size();
    //  Allocate memory for the cells
    cells.resize(dim_x);
    for ( size_t i = 0; i < dim_x; ++i )
        cells[i].resize(dim_y);
    //  Load Position& terrain type for each cell
    for (size_t j = 0 ; j < dim_y ; j++) 
        for (size_t i = 0 ; i < dim_x ; i++) 
            cells[i][j].setPosition(i,j),
            cells[i][j].setTerrain(cell_types[i][j]);
    //  Initialize creators
	for (player_t FACTION : factions)
    	for (unit_t UNIT : units)
            this->creatorID[FACTION][UNIT] = -1,
            this->creators[FACTION][UNIT] = 0;

}

void Board::addSandPosition(int x, int y){
    this->changed_sand_positions.push_back(Position(x,y));
}
void Board::clearSandPositions(){
    this->changed_sand_positions.clear();
}

void Board::addUnitCreator(player_t faction, building_t type){

    if (type == BARRACK) {
        this->creators[faction][FREMEN]++;
        this->creators[faction][INFANTRY]++;
        this->creators[faction][SARDAUKAR]++;
    }

    if (type == LIGHT_FACTORY) {
        this->creators[faction][TRIKE]++;
        this->creators[faction][HARVESTER]++;
    }

    if (type == HEAVY_FACTORY) {
        this->creators[faction][TANK]++;
        this->creators[faction][DEVASTATOR]++;
    }

}

void Board::removeUnitCreator(player_t faction, unit_t type){
    this->creators[faction][type]--;
}
int Board::getTotalCreators(player_t faction, unit_t type){
    if(type == UNDEFINED)
        return -1;
    return this->creators[faction][type];
}

status_t Board::canPlace(const Position& location, int dim_x,int dim_y) {
    //  See if any of the positions is occupied or cant build there
    for (size_t j = 0 ; j < dim_y ; j++)
        for (size_t i = 0 ; i < dim_x ; i++) 
            if (    
            this->cells[location.x+i][location.y+j].isOccupied()
            ||
            !(this->cells[location.x+i][location.y+j].canBuild())
            )
                return FAILED_TO_ADD;
    return SUCCESS;
}

bool Board::canDeposit(int x, int y,player_t faction){
    Position depositing_position(x,y);
    for (Position pos : this->deposit_positions[faction])
        if (depositing_position == pos)
            return true;
    return false;
}

bool Board::canHarvest(int x, int y){return cells[x][y].canHarvest();}
bool Board::canTraverse(int x, int y){return cells[x][y].canTraverse();}

bool Board::hasEnemy(int x, int y, player_t player_faction){
    if(this->cells[x][y].isOccupied() == false)
        return false;
    player_t faction = this->elements.at(this->cells[x][y].getID())->getFaction();
    return !(faction == player_faction);
}

void Board::dealDamage(int x, int y, int damage){
    std::unique_ptr<Selectable>& element = this->elements.at(this->cells[x][y].getID());
    element->receiveDamage(damage);
    std::cout << "Element LP" << element->getLP() <<std::endl;
    if(element->getLP() <= 0)
        this->deleteElement(element->getID());
}

void Board::deleteElement(int id){
    std::unique_ptr<Selectable>& element = this->elements.at(id);
    for(unit_t UNIT : units)
        if(element->canCreate(UNIT)) {
            this->removeUnitCreator(element->getFaction(),UNIT);
            if (element->getID() == this->creatorID[element->getFaction()][UNIT])
                this->creatorID[element->getFaction()][UNIT] = this->findNewCreator(element->getFaction(),UNIT);
        }
    for (Position pos : element->getPositions())
        this->cells[pos.x][pos.y].disoccupy();
    this->elements.erase(element->getID());
}

Cell& Board::getCell(int x, int y){
    return this->cells[x][y];
}

int Board::findNewCreator(player_t faction,unit_t unit){    
    for (auto& e : this->elements)
        if(e.second->getFaction() == faction)
            if(e.second->canCreate(unit))
                if (e.second->getID() != this->creatorID[faction][unit])
                    return e.second->getID();
    return -1;
}

std::unique_ptr<Selectable> & Board::getElementAt(int x, int y){
    int elementID = this->cells[x][y].getID();
    return this->elements.at(elementID);
}

size_t Board::get_width() {
    return (size_t) this->dim_x;
}
size_t Board::get_height() {
    return (size_t) this->dim_y;
}

std::vector<Position> Board::get_traversable_neighbors_of(Position pos, size_t distance) {

    std::vector<Position> valid_neighbors;

    for (int i = pos.x-distance; i <= pos.x+distance; i++) {
        for (int j = pos.y-distance; j <= pos.y+distance; j++) {            
            // if i == 0 and j == 0, this is "pos"
            if (i == pos.x && j == pos.y)
                continue;
            Position neighbor(i, j);
            // check if neighbor is out of bounds
            if (neighbor.x >= this->dim_x || neighbor.y >= this->dim_y)
                continue;
            if (neighbor.x < 0 || neighbor.y < 0)
                continue;                
            // check if neighbor is not traversable
            if (!(this->cells[neighbor.x][neighbor.y].canTraverse())||(this->cells[neighbor.x][neighbor.y].isOccupied())||(this->cells[neighbor.x][neighbor.y].getReserveID() != -1))                
                continue;
            valid_neighbors.push_back(neighbor);
        }
    }
    return valid_neighbors;
}

size_t Board::get_distance_between(Position pos1, Position pos2) {

    size_t x_dist = abs(pos2.x-pos1.x);
    size_t y_dist = abs(pos2.y-pos1.y);

    return x_dist > y_dist ? x_dist : y_dist;
}

void Board::addDepositPositions(player_t faction,std::vector<Position> & new_deposit_positions){
    for (Position pos : new_deposit_positions)
        this->deposit_positions[faction].push_back(pos);
}

std::vector<Position> Board::getDepositPositions(player_t faction){
    return this->deposit_positions[faction];
}

int Board::getCreator(player_t faction,unit_t type){

    return this->creatorID[faction][type];
}

void Board::makeCreator(int building_ID){
    for (unit_t UNIT : units){
        std::cout << "dfg" << std::endl;
        if(this->elements.at(building_ID)->canCreate(UNIT))
            this->creatorID[this->elements.at(building_ID)->getFaction()][UNIT] = building_ID;
    }
}

void Board::removeCreator(player_t faction, unit_t unit) {
        this->creatorID[faction][unit] = -1;
}

std::vector<Position> Board::getSurroundings(Position position, int e_dim_x, int e_dim_y){
    std::vector <Position> surroundings;
    for (int i = (position.x - 1); i <= (position.x + e_dim_x); i++) {
        for (int j = (position.y - 1); j <= (position.y + e_dim_y); j++) {
            // ignore internal positions
            if (((i < (position.x + e_dim_x)) && (i >= position.x)) && ((j < (position.y + e_dim_y)) && (j >= position.y)))
                continue;
            // check if out of bounds
            if ((i >= this->dim_x) || (j >= this->dim_y))
                continue;
            if ((i < 0) || (j < 0))
                continue;
            Position neighbor(i, j);
            surroundings.push_back(neighbor);
        }
    }
    return surroundings;
}
