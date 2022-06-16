#include "board.h"


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
    for (size_t j = 0 ; j < dim_y ; j++) {
        for (size_t i = 0 ; i < dim_x ; i++) {
            cells[i][j].setPosition(i,j);
            cells[i][j].setTerrain(cell_types[i][j]);
        }
    }
    this->creators.insert({HARVESTER,-1});
    this->creators.insert({TRIKE,-1});
}

status_t Board::canPlace(const Position& location, int dim_x,int dim_y) {
    //  See if any of the positions is occupaid or cant build there
    for (size_t j = 0 ; j < dim_y ; j++) {
        for (size_t i = 0 ; i < dim_x ; i++) {
            if (    
            this->cells[location.x+i][location.y+j].isOccupied()
            ||
            !(this->cells[location.x+i][location.y+j].canBuild())
            ){
                return FAILED_TO_ADD;
            }
        }
    }
    return SUCCESS;
}

bool Board::canDeposit(int x, int y,player_t faction){
    Position depositing_position(x,y);
    for (Position pos : this->deposit_positions)
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
    if(element->getLP() <= 0){
        Position element_position = element->getPosition(); 
        std::vector<Position> positions = element->getPositions();

        // TODO: refactor
        int destroyed_building_id = this->cells[x][y].getID();
        int unit_creator_id = -1;
        unit_creator_id = this->getCreator(TRIKE);
        if (destroyed_building_id == unit_creator_id)
            this->removeCreator(TRIKE);
        unit_creator_id = this->getCreator(HARVESTER);
        if (destroyed_building_id == unit_creator_id)
            this->removeCreator(HARVESTER);
        
        
        this->elements.erase(this->cells[x][y].getID());


        for (Position pos : positions)
            this->cells[pos.x][pos.y].disoccupy();
    }
}

Cell& Board::getCell(int x, int y){
    return this->cells[x][y];
}

std::unique_ptr<Selectable> & Board::getElementAt(int x, int y){
    int elementID = this->cells[x][y].getID();
    return this->elements.at(elementID);
}

void Board::print() {
    for (size_t j = 0 ; j < this->dim_y ; j++) {
        for (size_t i = 0 ; i < this->dim_x ; i++)
            this->cells[i][j].print();
        std::cout << '\n';
    }
    std::cout << '\n';
}

void Board::print(std::vector<Position> path) {

    std::vector <std::vector <bool>> paths_board(this->dim_x, std::vector<bool>(this->dim_y));

    for (Position position : path) {
        (paths_board)[position.x][position.y] = true;
        (paths_board)[position.x][position.y] = true;
    }

    for (size_t j = 0 ; j < this->dim_y ; j++) {
        for (size_t i = 0 ; i < this->dim_x ; i++) {
            if ((paths_board)[i][j]) {
                std::cout << "# ";
                continue;
            }
            this->cells[i][j].print();
        }
        std::cout << '\n';
    }
    std::cout << '\n';
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
            if (!(this->cells[neighbor.x][neighbor.y].canTraverse()))
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

void Board::addDepositPositions(std::vector<Position> & new_deposit_positions){
    for (Position pos : new_deposit_positions)
        this->deposit_positions.push_back(pos);
}

std::vector<Position> & Board::getDepositPositions(){
    return this->deposit_positions;
}

int Board::getCreator(unit_t type){
    return this->creators[type];
}

void Board::makeCreator(int building_ID){
    if (this->elements.at(building_ID)->getName() == "Refinery")
        this->creators[HARVESTER] = building_ID; 
    if (this->elements.at(building_ID)->getName() == "Barrack"){
        this->creators[TRIKE] = building_ID;     
    }
    std::cout << this->elements.at(building_ID)->getName() << " of ID: " << building_ID << " is now a creator" << std::endl;
}

void Board::removeCreator(unit_t unit) {

    switch (unit)
    {
    case TRIKE:
        this->creators[TRIKE] = -1;
        break;
    case HARVESTER:
        this->creators[HARVESTER] = -1;
        break;
    default:
        break;
    }
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
