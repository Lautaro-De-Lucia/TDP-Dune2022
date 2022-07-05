#pragma once

#include <iostream>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"

#define DIM_X_HUD 191

struct Camera {

    int pos_x;
    int pos_y;
    int height;
    int width;
    int screen_height;
    int screen_width;
    SDL2pp::Rect view;
    //SDL2pp::Window& window;
    //SDL2pp::Renderer& renderer;

    Camera(int pos_x, int pos_y, int width, int height, int screen_width, int screen_height);
    void move(int x,int y);
};
