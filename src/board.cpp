#include "board.h"

Board::Board(std::vector<std::vector<cell_t>> cell_types) :
cells() {
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
}

status_t Board::place(const Position& location, int dim_x,int dim_y,player_t faction) {
    //  See if any of the positions to build is already occupied
    for (size_t j = 0 ; j < dim_y ; j++) {
        for (size_t i = 0 ; i < dim_x ; i++) {
            if (this->cells[location.x+i][location.y+j].isOccupied() == true){
                std::cout << "Location: (" << location.x+i << "," << location.y+j << ") was occupied!" << std::endl;      
                return FAILED_TO_ADD;
            }
        }
    }
    //  If not, mark them as occupied
    for (size_t j = 0 ; j < dim_y ; j++){ 
        for (size_t i = 0 ; i < dim_x ; i++){
            this->cells[location.x+i][location.y+j].occupy(faction);
            std::cout << "Location: (" << location.x+i << "," << location.y+j << ") is now occupied" << std::endl;      
            std::cout << "With the faction number: " << faction << std::endl;
        }
    }
            
        
    return SUCCESS;
}

Cell& Board::getCell(int x, int y){
    return this->cells[x][y];
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

void Board::move_unit(Position from, Position to, player_t faction) {
    (this->cells)[from.x][from.y].disoccupy();
    (this->cells)[to.x][to.y].occupy(faction);
}