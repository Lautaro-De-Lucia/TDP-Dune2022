#pragma once

#include <SDL2pp/SDL2pp.hh>
#include "client_gamehud_button.h"
#include "common_utils.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

class GameHud {
 private:
   SDL2pp::Texture texture;
   std::vector<Button> build_buttons;
   std::vector<Button> unit_buttons;
   SDL2pp::Rect dRect;
 public:
   GameHud(SDL2pp::Renderer& renderer);
   void render(SDL2pp::Renderer& renderer);
   void addUnitButton(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id);
   void addBuildButton(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id);
   int checkUnit(int& x, int& y);
   int checkBuild(int& x, int& y);
   bool clickOnHud(int& x, int& y);
};
