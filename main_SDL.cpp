#include <iostream>
#include "player.h"
#include "client_player.h"
#include "common_utils.h"
#include <SDL2pp/SDL2pp.hh>

#define INIT_SPICE 5000
#define INIT_CSPICE 5000
#define INIT_ENERGY 3000
#define INIT_CENERGY 5000

#define MAP_DIM_X 80
#define MAP_DIM_Y 45

std::vector<std::vector<cell_t>> generate_random_map (){
    std::vector<std::vector<cell_t>> map_cells(MAP_DIM_X);
    for (size_t i = 0 ; i < MAP_DIM_X ; i++) {
        map_cells[i].resize(MAP_DIM_Y);
    }
    for (size_t j = 0 ; j < MAP_DIM_Y ; j++) {
        for (size_t i = 0 ; i < MAP_DIM_X ; i++) {
            map_cells[i][j] = ROCK;
        }
    }
    return map_cells;
}

int main (void){
    SDL2pp::Window game_window("Dune II",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_ALWAYS_ON_TOP);
    SDL2pp::Renderer game_renderer(game_window, -1, SDL_RENDERER_ACCELERATED);
    Board board(45,25);
    Player server(board,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY);
    std::vector<std::vector<cell_t>> cells = generate_random_map();
    CPlayer(game_window,game_renderer,INIT_SPICE,INIT_CSPICE,INIT_ENERGY,INIT_CENERGY,cells);
    server.run();
}