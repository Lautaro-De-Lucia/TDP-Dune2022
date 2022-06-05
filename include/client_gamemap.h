#pragma once

#include <iostream>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"

#define TILE_DIM 16

class MapCell{
    Position position;
    SDL2pp::Texture texture;

    void render(SDL2pp::Renderer & renderer);
};
class GameMap {
private:
    std::vector<std::vector<MapCell>> cells;
public:
    GameMap(std::vector<std::vector<cell_t>> & cells);
};
