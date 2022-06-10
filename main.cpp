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
#include <vector> 

#define INIT_SPICE 5000
#define INIT_CSPICE 5000
#define INIT_ENERGY 3000
#define INIT_CENERGY 5000

#define MAP_DIM_X 80
#define MAP_DIM_Y 45

std::vector<std::vector<cell_t>> generate_random_map () {
    //  Create space for the map
    std::vector<std::vector<cell_t>> map_cells(MAP_DIM_X);
    for (size_t i = 0 ; i < MAP_DIM_X ; i++) {
        map_cells[i].resize(MAP_DIM_Y);
    }
    //  
    for (size_t j = 0 ; j < MAP_DIM_Y ; j++) {
        for (size_t i = 0 ; i < MAP_DIM_X ; i++) {
            //if (j >= 0 && j < 30 && i >= 0  && i < 40){
                map_cells[i][j] = ROCK;
              //  continue;
            //}    
            //map_cells[i][j] = (cell_t) (rand() % 4);
        }
    }
    return map_cells;
}

void greet(SDL2pp::Renderer &game_renderer) {
    std::string text = "Loading map... please wait";
    SDL2pp::SDLTTF ttf;
    SDL2pp::Font font("../src/ui/resources/fonts/beyond-mars.ttf", 64);
	SDL2pp::Texture text_sprite(game_renderer, font.RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));
	game_renderer.Copy(text_sprite, SDL2pp::NullOpt, SDL2pp::Rect((SCREEN_WIDTH/2 - text_sprite.GetWidth()/2), 
        SCREEN_HEIGHT/2, text_sprite.GetWidth(), text_sprite.GetHeight()));
    game_renderer.Present();
}

void run_server(Player &server) {
    server.run();
}
void run_sdl() {

    SDL2pp::Window game_window("Dune II",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_ALWAYS_ON_TOP);
    SDL2pp::Renderer game_renderer(game_window, -1, SDL_RENDERER_ACCELERATED);
    greet(game_renderer);
    std::vector<std::vector<cell_t>> cells = generate_random_map();

    Camera cam(0,0,640,360,1280,720);
    CPlayer client_player(cam,game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cells);
    Board board(cells);

    Player server(HARKONNEN,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,board,client_player);
    std::thread thread_server(run_server, std::ref(server));

    bool running = true;
    bool drag = false;
    
    int x, y;
    int x_coord, y_coord;
    int x_drag, y_drag;
    int x_coord_drag, y_coord_drag;
    int x_drag_finish, y_drag_finish;

    int unit, building;
    bool build = false;
    SDL_Event event;
    while(running) {   
        while (SDL_PollEvent( &event )) {
            if (drag == false) {
                SDL_GetMouseState( &x, &y);
                x_coord = (x/(TILE_DIM*2)) + (cam.pos_x/TILE_DIM);
                y_coord = (y/(TILE_DIM*2)) + (cam.pos_y/TILE_DIM);
            }
            switch (event.type) {
                case SDL_QUIT: //User requests quit
                    running = false;
                    SDL_Quit();
                    break;
                case SDL_MOUSEMOTION: // User moves cursor
                    if (drag == true) {
                        SDL_GetMouseState( &x, &y);
                        x_drag_finish = (x/(TILE_DIM*2)) + (cam.pos_x/TILE_DIM);
                        y_drag_finish = (y/(TILE_DIM*2)) + (cam.pos_y/TILE_DIM);
                    } break;
                case SDL_MOUSEBUTTONUP:  // User releases mouseclick
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (build) {
                            server.addAction({CREATE_BUILDING, building, x_coord, y_coord});
                            build = false;
                        }
                        else {
                            if (client_player.checkHud(x, y)) {
                                unit = client_player.checkUnit(x, y);
                                if (unit != -1) server.addAction({CREATE_UNIT, unit});
                                else {  
                                    building = client_player.checkBuild(x, y);
                                    if (building != -1) build = true;
                                }
                            } else server.addAction({MOUSE_LEFT_CLICK, x_coord, y_coord});
                        } 
                    } else if (event.button.button == SDL_BUTTON_RIGHT) server.addAction({MOUSE_RIGHT_CLICK, x_coord, y_coord});
                    break;
                case SDL_KEYDOWN: // User unreleases key
                    if(event.key.keysym.sym == SDLK_SPACE) {
                        if (drag == false) {
                            drag = true;
                            SDL_GetMouseState( &x_drag, &y_drag);
                            x_coord_drag = (x_drag/(TILE_DIM*2)) + (cam.pos_x/TILE_DIM);
                            y_coord_drag = (y_drag/(TILE_DIM*2)) + (cam.pos_y/TILE_DIM);
                        } break; 
                    }
                case SDL_KEYUP: // User releases key
                    if(event.key.keysym.sym == SDLK_SPACE) {
                        if (drag == true) {
                            server.addAction({MOUSE_SELECTION, 
                                std::min(x_coord_drag, x_drag_finish), 
                                std::max(x_coord_drag, x_drag_finish),
                                std::min(y_coord_drag, y_drag_finish), 
                                std::max(y_coord_drag, y_drag_finish)});
                            drag = false;
                        } break;
                }
            }
        }
    }
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
