#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <SDL2pp/SDL2pp.hh>

#include "client_camera.h"

#include "common_utils.h"

extern Camera cam;

#define TILE_DIM 16
#define ROCK_PATH "/mapsprites/rock_1.png"
#define SAND_PATH "/mapsprites/sand_1.png"
#define DUNE_PATH "/mapsprites/dune_1.png"
#define PIT_PATH "/mapsprites/pit_1.png"

#define SAND_PATH_EMPTY "/mapsprites/s0.png"
#define SAND_PATH_LITTLE "/mapsprites/s1.png"
#define SAND_PATH_SOME "/mapsprites/s2.png"
#define SAND_PATH_FULL "/mapsprites/s3.png"

class MapCell{
 private:    
    Position position;
    SDL2pp::Texture texture;
 public:
    MapCell( SDL2pp::Renderer& renderer, const std::string& path,size_t pos_x, size_t pos_y);
    void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
};
class GameMap {
 private:
    int dim_x;
    int dim_y;
    std::vector<std::vector<std::unique_ptr<MapCell>>> map_cells;
    std::vector<Position> destroyed_positions;
 public:
    GameMap(SDL2pp::Renderer& renderer,std::vector<std::vector<std::string>>& cells);
    void updateCell(SDL2pp::Renderer& renderer,int pos_x, int pos_y,int spice);
    void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
};
