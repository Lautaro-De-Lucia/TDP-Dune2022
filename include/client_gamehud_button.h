#pragma once

#include <SDL2pp/SDL2pp.hh>
#include <string>

#include "common_utils.h"

#define DIM_BUTTON 50

class Button {
 private:
    SDL2pp::Texture texture;
    int x;
    int y;
    int id;
    SDL2pp::Rect dRect;
 public:
    Button(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id);
    void render(SDL2pp::Renderer& renderer);
    int checkIntersection(int& x, int& y);
    void setTextureColor(double percentage,color_t color);
    int getID(){return this->id;}
};
