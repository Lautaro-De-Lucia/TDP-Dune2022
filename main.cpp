#include <iostream>
#include <stdlib.h>
#include "player.h"
#include "client_player.h"
#include "common_utils.h"
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

void run_sdl() {
    SDL2pp::Window game_window("Dune II",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_ALWAYS_ON_TOP);
    SDL2pp::Renderer game_renderer(game_window, -1, SDL_RENDERER_ACCELERATED);
    Board board(45,25);
    std::vector<std::vector<cell_t>> cells = generate_random_map();
    CPlayer client_player(game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cells);
    client_player.renderMap(game_renderer);
    game_renderer.Present();
    State air_trap_state;
    air_trap_state.ID = 0;
    air_trap_state.LP = 100;
    air_trap_state.position = Position(20,20);
    air_trap_state.selected = false;    
    client_player.addElement(AIR_TRAP,air_trap_state,game_renderer);
    State refinery_state;
    refinery_state.ID = 1;
    refinery_state.LP = 100;
    refinery_state.position = Position(30,30);
    refinery_state.selected = false;   
    client_player.addElement(REFINERY,refinery_state,game_renderer);
    State barrack_state;
    barrack_state.ID = 2;
    barrack_state.LP = 100;
    barrack_state.position = Position(30,20);
    barrack_state.selected = false;   
    client_player.addElement(BARRACK,barrack_state,game_renderer);
    State trike_state;
    trike_state.ID = 3;
    trike_state.LP = 100;
    trike_state.position = Position(40,20);
    trike_state.selected = false;   
    client_player.addElement(TRIKE,trike_state,game_renderer);
    State harvester_state;
    harvester_state.ID = 4;
    harvester_state.LP = 100;
    harvester_state.position = Position(40,30);
    harvester_state.selected = false;   
    client_player.addElement(HARVESTER,harvester_state,game_renderer);
    std::vector<State> states;
    states.push_back(air_trap_state);
    states.push_back(refinery_state);
    states.push_back(barrack_state);
    states.push_back(trike_state);
    states.push_back(harvester_state);
    client_player.update(states,game_renderer);
    game_renderer.Present();
    for(size_t i = 0 ; i < 5 ; i++) {
        states[3].position = Position (40,20+i);
        states[4].position = Position (40,30-i);
        game_renderer.Clear();
        client_player.renderMap(game_renderer);
        client_player.update(states,game_renderer);
        game_renderer.Present();
        sleep(1);
    }
    Player server(board,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY);
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

}

