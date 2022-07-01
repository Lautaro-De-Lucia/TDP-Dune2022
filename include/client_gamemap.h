#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <SDL2pp/SDL2pp.hh>

#include "client_camera.h"
#include "client_texturehandler.h"

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
    std::string type;
 public:
    MapCell() = default;
    void setPosition(int pos_x, int pos_y);
    void setType(const char * new_type);
    void render(SDL2pp::Renderer& renderer,TextureHandler & textures, int cam_pos_x, int cam_pos_y);
};
class GameMap {
 private:
    int dim_x;
    int dim_y;
    std::vector<std::vector<MapCell>> map_cells;
 public:
    GameMap(SDL2pp::Renderer& renderer,std::vector<std::vector<std::string>>& cells);
    void updateCell(SDL2pp::Renderer& renderer,int pos_x, int pos_y,int spice);
    void render(SDL2pp::Renderer& renderer,TextureHandler & texture, int cam_pos_x, int cam_pos_y);
};
