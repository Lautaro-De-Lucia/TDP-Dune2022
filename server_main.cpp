#include <iostream>
#include <fstream>

#include "server_server.h"

#define MAP_FILE "/dune.map"

#define MAP_DIM_X 80
#define MAP_DIM_Y 45

#define TOTAL_TILE_SPRITES 8

#define SERVICE_NAME "http-alt"

std::vector<std::vector<cell_t>> generate_server_map (std::string tile_map_file) {
    //  Produce cell matrix
    std::vector<std::vector<cell_t>> cells;
    cells.resize(MAP_DIM_X);
	for ( size_t i = 0; i < MAP_DIM_X; i++ )
		cells[i].resize(MAP_DIM_Y);
    //  Load .map
	std::ifstream map(tile_map_file.c_str());
	if (map.fail()){
		std::cout << "Failed to load map" << std::endl; 
		exit(1);
	}
	//	Read & load tiles
	for(size_t j = 0 ; j < MAP_DIM_Y ; j++ ){
		for (size_t i = 0 ; i < MAP_DIM_X ; i++){
			std::string cell;
			map >> cell;
            int type;
            if (cell[0] == 'r')
                type = ROCK;
            if (cell[0] == 's'){
                if(cell[1] == '0')
                    type = SAND_EMPTY; 
                if(cell[1] == '1')
                    type = SAND_LITTLE;
                if(cell[1] == '2')
                    type = SAND_SOME;
                if(cell[1] == '3')
                    type = SAND_FULL;                               
            }
            if (cell[0] == 'd')
                type = DUNE;
            if (cell[0] == 'p')
                type = PIT;    
			if (map.fail()){
				std::cout << "Unexpected EOF on tile map" << std::endl; 
				exit(1);
			}
			if (type < 0 || type > TOTAL_TILE_SPRITES){
				std::cout << "This ain't a valid tile number, dumbass" << std::endl; 
				exit(1);
			}
			//	Load cell to board
			cells[i][j]= (cell_t) type;
		}
	}
	map.close();
    return cells;
}

int main() {

    std::vector<std::vector<cell_t>> cells = generate_server_map(DATA_PATH MAP_FILE);
    Server server(SERVICE_NAME,cells);
    std::thread server_thread(&Server::run, &server);
    while (1) {

    }    //  Hold for input
    server.stop();
    server_thread.join();
}
