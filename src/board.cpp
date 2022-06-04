#include "board.h"

Board::Board(uint16_t dim_x, uint16_t dim_y)
: dim_x(dim_x), dim_y(dim_y), cells() {
    //  Allocate memory in the stack
        cells.resize(dim_x);
        for ( size_t i = 0; i < dim_x; ++i )
            cells[i].resize(dim_y);
    //  Load values for cell positions
    for (size_t j = 0 ; j < dim_y ; j++) {
        for (size_t i = 0 ; i < dim_x ; i++) {
            cells[i][j].setPosition(i,j);
        }
    }    
}

status_t Board::place(const Position & location, uint8_t dim_x,uint8_t dim_y) {
    //  See if any of the positions to build is already occupied
    for (size_t j = 0 ; j < dim_y ; j++) {
        for (size_t i = 0 ; i < dim_x ; i++) {
            if (this->cells[location.x+i][location.y+j].isOccupied() == true)
                return FAILED_TO_ADD;
        }
    }
    //  If not, mark them as occupied
    for (size_t j = 0 ; j < dim_y ; j++) 
        for (size_t i = 0 ; i < dim_x ; i++) 
            this->cells[location.x+i][location.y+j].occupy();
        
    return SUCCESS;
}

Cell& Board::getCell(uint16_t x, uint16_t y){
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
                std::cout << "#";
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

