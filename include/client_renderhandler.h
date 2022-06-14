#pragma once

#include <iostream>
#include <SDL2pp/SDL2pp.hh>

enum move_direction {
    TOP = 0,
    TOP_RIGHT = 1,
    RIGHT = 2,
    BOTTOM_RIGHT = 3,
    BOTTOM = 4,
    BOTTOM_LEFT = 5,
    LEFT = 6,
    TOP_LEFT = 7
};


class RenderHandler {
    private:
    public:
    void renderMovable(
        SDL2pp::Texture & texture,
        SDL2pp::Renderer & renderer,
        move_direction dir,
        int pos_x,
        int pos_y, 
        int cam_pos_x,
        int cam_pos_y,
        int tile_size,
        int sprite_cut_x, 
        int sprite_cut_y, 
        int size);
    void renderStatic(SDL2pp::Texture & texture,int size_x, int size_y);
};