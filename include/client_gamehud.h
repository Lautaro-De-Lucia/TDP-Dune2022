#pragma once

#include <SDL2pp/SDL2pp.hh>

#include "client_gamehud_button.h"
#include "client_printer.h"

#include "common_utils.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

#define BUTTON_TEXTURE_PATH "/frameworks/"


class GameHud {
 private:
   SDL2pp::Texture texture;
   std::vector<Button> build_buttons;
   std::vector<Button> unit_buttons;
   std::map<int,std::string> button_info_paths;
   SDL2pp::Rect dRect;
   CPrinter spice_printer;
   CPrinter energy_printer;
   int spice; 
   int c_spice;
   int energy;
 public:
   GameHud(player_t faction, SDL2pp::Renderer& renderer);
   void render(SDL2pp::Renderer& renderer);
   void update(int spice,int c_spice, int energy);
   void addUnitButton(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id);
   void addBuildButton(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id);
   int checkUnit(int& x, int& y);
   int checkBuild(int& x, int& y);
   bool clickOnHud(int& x, int& y);
  std::string getButtonInfoPath(hud_button_t button,int id);
};
