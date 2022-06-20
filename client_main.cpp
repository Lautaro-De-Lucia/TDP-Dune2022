#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include "client_player.h"
#include "common_utils.h"
#include "client_camera.h"
#include <SDL2pp/SDL2pp.hh>

#include "src/ui/vistas.h"
#include <QApplication>
#include <QtConcurrent/QtConcurrent>

#include <functional>

#define INIT_SPICE 100000
//#define INIT_SPICE 15000
#define INIT_CSPICE 20000
#define INIT_ENERGY 3000
#define INIT_CENERGY 5000

#define MAP_FILE "/dune.map"

#define MAP_DIM_X 80
#define MAP_DIM_Y 45

#define TOTAL_TILE_SPRITES 5

#define HOST_ADDRESS "localhost"
#define SERVICE "http-alt"

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

void greet(SDL2pp::Renderer& game_renderer) {
    std::string text = "Loading map... please wait";
    SDL2pp::SDLTTF ttf;
    SDL2pp::Font font("../src/ui/resources/fonts/beyond-mars.ttf", 64);
	SDL2pp::Texture text_sprite(game_renderer, font.RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));
	game_renderer.Copy(text_sprite, SDL2pp::NullOpt, SDL2pp::Rect((SCREEN_WIDTH/2 - text_sprite.GetWidth()/2), 
        SCREEN_HEIGHT/2, text_sprite.GetWidth(), text_sprite.GetHeight()));
    game_renderer.Present();
}


void run_sdl(const int* _faction) {

    //  This is only to initialize TTF
	SDL2pp::SDLTTF ttf;

    player_t faction = (player_t) *(_faction); //Esto debe poder definirse desde el menú de Qt

    SDL2pp::Window game_window("Dune II",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT,0);
    SDL2pp::Renderer game_renderer(game_window, -1, SDL_RENDERER_ACCELERATED);

    std::cout << "faction is: " << faction << std::endl;

    std::vector<std::vector<std::string>> cell_paths = generate_client_map(DATA_PATH MAP_FILE);    
    Camera cam(CAMERA_INITIAL_POS_X*(faction-1),CAMERA_INITIAL_POS_Y*(faction-1),CAMERA_WIDTH,CAMERA_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT);
    Player client_player(faction,HOST_ADDRESS,SERVICE,cam,game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cell_paths);

    client_player.play();
};


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("./src/resources/img/icon.png"));
    MainWindow w;
    w.show();

    int __faction = -1;
    int* _faction = &__faction;

    LobbyWindow l;
    FactionWindow f(nullptr,_faction);

    QObject::connect(&w, &MainWindow::jugar, &w, &QMainWindow::close);
    //QObject::connect(&w, &MainWindow::jugar, &l, &QMainWindow::show);
    QObject::connect(&w, &MainWindow::jugar, &f, &QMainWindow::show);

    //QObject::connect(&l, &LobbyWindow::jugar, &l, [=]() {QtConcurrent::run(run_sdl);});
    QObject::connect(&f, &FactionWindow::jugar, &f, &QMainWindow::close);



    QObject::connect(&f, &FactionWindow::jugar, &f, [=]() {QtConcurrent::run(run_sdl, std::ref(_faction));});

    return a.exec();

    /*
    run_sdl();

    return 0;
    */
}
