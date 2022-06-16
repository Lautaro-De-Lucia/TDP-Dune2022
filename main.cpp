#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "client_player.h"
#include "common_utils.h"
#include "client_camera.h"
#include <SDL2pp/SDL2pp.hh>

#include "src/ui/vistas.h"
#include <QApplication>
#include <QtConcurrent/QtConcurrent>


#define INIT_SPICE 100000
//#define INIT_SPICE 15000
#define INIT_CSPICE 20000
#define INIT_ENERGY 3000
#define INIT_CENERGY 5000

#define MAP_FILE "/dune.map"

#define MAP_DIM_X 80
#define MAP_DIM_Y 45

#define TOTAL_TILE_SPRITES 5

std::vector<std::vector<cell_t>> generate_server_map (std::string tile_map_file){
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
            if (cell[0] == 's')
                type = SAND;          
            if (cell[0] == 'd')
                type = DUNE;
            if (cell[0] == 'p')
                type = PIT;    
			if (map.fail()){
				std::cout << "Unexpected EOF on tile map" << std::endl; 
				exit(1);
			}
			if (type < 0 && type > TOTAL_TILE_SPRITES){
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

std::vector<std::vector<std::string>> generate_client_map (std::string tile_map_file){
    //  Produce cell matrix
    std::vector<std::vector<std::string>> cells;
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
            std::string type;
			map >> type;
			if (map.fail()){
            	std::cout << "Unexpected EOF on tile map" << std::endl; 
				exit(1);
			}
			//	Load cell to board
            std::string cell_data;
            cell_data.append(DATA_PATH);
            cell_data.append("/mapsprites/");
            cell_data.append(type);
            cell_data.append(".png");
			cells[i][j]= cell_data;
		}
	}
	map.close();
    return cells;
}

std::vector<std::vector<cell_t>> generate_random_map () {
    //  Create space for the map
    std::vector<std::vector<cell_t>> map_cells(MAP_DIM_X);
    for (size_t i = 0 ; i < MAP_DIM_X ; i++) {
        map_cells[i].resize(MAP_DIM_Y);
    }
    for (size_t j = 0 ; j < MAP_DIM_Y ; j++) {
        for (size_t i = 0 ; i < MAP_DIM_X ; i++) {
                map_cells[i][j] = ROCK;
        }
    }

    map_cells[30][30] = PIT;
    map_cells[30][31] = PIT;
    map_cells[30][32] = PIT;

    map_cells[20][15] = SAND;
    map_cells[20][16] = SAND;
    map_cells[20][17] = SAND;
    map_cells[21][15] = SAND;
    map_cells[21][16] = SAND;
    map_cells[21][17] = SAND;
    map_cells[22][15] = SAND;
    map_cells[22][16] = SAND;
    map_cells[22][17] = SAND;

    return map_cells;
    
}

void greet(SDL2pp::Renderer& game_renderer) {
    std::string text = "Loading map... please wait";
    SDL2pp::SDLTTF ttf;
    SDL2pp::Font font("../src/ui/resources/fonts/beyond-mars.ttf", 64);
	SDL2pp::Texture text_sprite(game_renderer, font.RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));
	game_renderer.Copy(text_sprite, SDL2pp::NullOpt, SDL2pp::Rect((SCREEN_WIDTH/2 - text_sprite.GetWidth()/2), 
        SCREEN_HEIGHT/2, text_sprite.GetWidth(), text_sprite.GetHeight()));
    game_renderer.Present();
}


void run_sdl() {

    //  This is only to initialize TTF
	SDL2pp::SDLTTF ttf;

    SDL2pp::Window game_window("Dune II",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT,0);
    SDL2pp::Renderer game_renderer(game_window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<std::vector<std::string>> cell_paths = generate_client_map(DATA_PATH MAP_FILE);
    std::vector<std::vector<cell_t>> cells = generate_server_map(DATA_PATH MAP_FILE);
    Camera cam(0,200,640,360,1280,720);
/*    
    while(true){
    std::vector<State> server_data;
    std::vector<std::vector<std::string>> cell_paths = generate_client_map(DATA_PATH MAP_FILE);
    CPlayer client_player(cam,game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cell_paths);
    client_player.update(server_data,1000,1000);
    sleep(1);
    }
    
*/
    CPlayer client_player(cam,game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cell_paths);
    Player server(HARKONNEN,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cells,client_player);
    server.run();

};


int main(int argc, char *argv[]) {
    /*
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("./src/resources/img/icon.png"));
    MainWindow w;
    w.show();
    LobbyWindow l;

    QObject::connect(&w, &MainWindow::jugar, &w, &QMainWindow::close);
    QObject::connect(&w, &MainWindow::jugar, &l, &QMainWindow::show);
    QObject::connect(&l, &LobbyWindow::jugar, &l, [=]() {QtConcurrent::run(run_sdl);});

    return a.exec();
    */
    run_sdl();
    return 0;
}
