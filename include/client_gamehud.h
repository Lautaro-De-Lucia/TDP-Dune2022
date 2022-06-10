#pragma once

#include <SDL2pp/SDL2pp.hh>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

class GameHud {
 private:
    SDL2pp::Texture texture;
 public:
    GameHud(SDL2pp::Renderer & renderer);
    void render(SDL2pp::Renderer & renderer);
};
