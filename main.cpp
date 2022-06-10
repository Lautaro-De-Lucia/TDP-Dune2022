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
            if (j > 15 && j < 30 && i > 20  && i < 60){
                map_cells[i][j] = ROCK;
                continue;
            }    
            map_cells[i][j] = (cell_t) (rand() % 4);
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
    Camera cam(0,0,640,360,1280,720);

    Board board(45,25);

    std::vector<std::vector<cell_t>> cells = generate_random_map();
    CPlayer client_player(cam,game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cells);

    Player server(INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY, client_player);
    std::thread thread_server(run_server, std::ref(server));

    bool running = true;
    bool drag = false;
    int x, y;
    int c_x, c_y;
    int c_x2, c_y2;
    SDL_Event event;
    while(running) {   
        while (SDL_PollEvent( &event )) {
            if (drag == false) {
                SDL_GetMouseState( &x, &y);
                c_x = (x/(TILE_DIM*2)) + (cam.pos_x/TILE_DIM);
                c_y = (y/(TILE_DIM*2)) + (cam.pos_y/TILE_DIM);
            }
            switch (event.type) {
                //User requests quit
                case SDL_QUIT:
                    running = false;
                    SDL_Quit();
                    break;

                case SDL_MOUSEMOTION:
                    if (drag == true) {
                        SDL_GetMouseState( &c_x2, &c_y2 );
                        c_x2 = (c_x2/(TILE_DIM*2)) + (cam.pos_x/TILE_DIM);
                        c_y2 = (c_y2/(TILE_DIM*2)) + (cam.pos_y/TILE_DIM);
                    } break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                            if (drag == true) {
                                //server.handleSelection(min(c_x, c_x2), max(c_x, c_x2),
                                    // min(c_y, c_y2), max(c_y, c_y2));
                            } else {
                                client_player.checkBuild(x, y);
                                client_player.checkUnit(x, y);
                                //server.handleLeftClick();
                            }
                        }
                        else if (event.button.button == SDL_BUTTON_RIGHT) {
                            //server.handleRightClick();
                        } break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_SPACE) {
                        drag = true; break; }
                case SDL_KEYUP:
                    if(event.key.keysym.sym == SDLK_SPACE){
                        drag = false; break; }
            }
        }
    }
    thread_server.join();
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
