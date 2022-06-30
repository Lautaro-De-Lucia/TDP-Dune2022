#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include "client_player.h"
#include "client_audioplayer.h"
#include "common_utils.h"
#include "client_camera.h"
#include <SDL2pp/SDL2pp.hh>

#include "src/ui/vistas.h"
#include <QApplication>
#include <QtConcurrent/QtConcurrent>

#include <functional>

#define INIT_SPICE 2000
#define INIT_CSPICE 2000
#define INIT_ENERGY 3000
#define INIT_CENERGY 5000

#define MAP_FILE "/dune.map"

#define MAP_DIM_X 80
#define MAP_DIM_Y 45

#define TOTAL_TILE_SPRITES 5

#define CMD_ARG_IP 1
#define CMD_ARG_PORT 2
#define MAX_CMD_ARGS 3

std::vector<std::vector<std::string>> generate_client_map (std::string tile_map_file){
    //  Produce cell matrix
    std::vector<std::vector<std::string>> cells;
    cells.resize(MAP_DIM_X);
	for ( size_t i = 0; i < MAP_DIM_X; i++ )
		cells[i].resize(MAP_DIM_Y);
    //  Load .map
	std::ifstream map(tile_map_file.c_str());
	if (map.fail()){
		std::cerr << "Failed to load map" << std::endl; 
		exit(1);
	}
	//	Read & load tiles
	for(size_t j = 0 ; j < MAP_DIM_Y ; j++ ){
		for (size_t i = 0 ; i < MAP_DIM_X ; i++){
            std::string type;
			map >> type;
			if (map.fail()){
            	std::cerr << "Unexpected EOF on tile map" << std::endl; 
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


void run_sdl(const int* _faction, std::string host_name, std::string service_name) {

    while (_faction == nullptr){}
    player_t faction = (player_t) *(_faction);
    
    //  This is only to initialize TTF
	SDL2pp::SDLTTF ttf;
    SDL2pp::Window game_window("Dune II",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT,0);
    SDL2pp::Renderer game_renderer(game_window, -1, SDL_RENDERER_ACCELERATED);

    int init_cam_pos_x;
    int init_cam_pos_y;

    switch (faction)
    {
    case ATREIDES:
        init_cam_pos_x = 0;
        init_cam_pos_y = 0;
        break;
    case HARKONNEN:
        init_cam_pos_x = 70*TILE_DIM;
        init_cam_pos_y = 35*TILE_DIM;
        break;
    case ORDOS:
        init_cam_pos_x = 0;
        init_cam_pos_y = 35*TILE_DIM;
        break;    
    default:
        break;
    }

    Camera cam(init_cam_pos_x,init_cam_pos_y,CAMERA_WIDTH,CAMERA_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT);

    const char* _host_name = host_name.c_str();
    const char* _service_name = service_name.c_str();

    std::vector<std::vector<std::string>> cell_paths = generate_client_map(DATA_PATH MAP_FILE);    
    Player client_player(faction,_host_name,_service_name,cam,game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cell_paths);

    client_player.play();
};


int main(int argc, char *argv[]) {

    if (argc != MAX_CMD_ARGS)
        return 1;

    const char* host_name = argv[CMD_ARG_IP];
    const char* service_name = argv[CMD_ARG_PORT];

    std::string _host_name(host_name);
    std::string _service_name(service_name);

    std::string _program_name = "./client";
    int _argc = 1;
    char *_argv[] = {"./client"};

    QApplication a(_argc, _argv);
    a.setWindowIcon(QIcon("./src/resources/img/icon.png"));
    MainWindow w;
    w.show();

    int __faction = -1;
    int* _faction = &__faction;

    LobbyWindow l;
    FactionWindow f(nullptr,_faction);
    CreditsWindow c;

    QObject::connect(&w, &MainWindow::jugar, &w, &QMainWindow::close);
    QObject::connect(&w, &MainWindow::credits, &w, &QMainWindow::close);

    QObject::connect(&w, &MainWindow::jugar, &f, &QMainWindow::show);
    QObject::connect(&w, &MainWindow::credits, &c, &QMainWindow::show);
    
    QObject::connect(&f, &FactionWindow::jugar, &f, &QMainWindow::close);
    QObject::connect(&c, &CreditsWindow::volver, &c, &QMainWindow::close);
    QObject::connect(&c, &CreditsWindow::volver, &w, &QMainWindow::show);
    
    QObject::connect(&f, &FactionWindow::jugar, &f, [=]() {QtConcurrent::run(run_sdl, std::ref(_faction), _host_name, _service_name);});

    return a.exec();
}
