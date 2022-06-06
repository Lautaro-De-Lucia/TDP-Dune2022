#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"

#define TILE_DIM 16
#define ROCK_PATH "/mapsprites/rock_1.png"
#define SAND_PATH "/mapsprites/sand_1.png"
#define DUNE_PATH "/mapsprites/dune_1.png"
#define PIT_PATH "/mapsprites/pit_1.png"


class MapCell{
private:    
    Position position;
    SDL2pp::Texture texture;
public:
    MapCell( SDL2pp::Renderer & renderer, const std::string& path,size_t pos_x, size_t pos_y);
    void render(SDL2pp::Renderer & renderer);
};
class GameMap {
private:
    std::vector<MapCell> map_cells;
public:
    GameMap(SDL2pp::Renderer & renderer,std::vector<std::vector<cell_t>> & cells);
    void render(SDL2pp::Renderer & renderer);
};
