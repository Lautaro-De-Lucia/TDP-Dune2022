#pragma once

#include <iostream>
#include <SDL2pp/SDL2pp.hh>


struct Camera {

    int pos_x;
    int pos_y;
    int height;
    int width;
    int screen_height;
    int screen_width;
    SDL2pp::Rect view;

    Camera(int pos_x, int pos_y, int width, int height, int screen_width, int screen_height);
    void move(int x,int y);

};
