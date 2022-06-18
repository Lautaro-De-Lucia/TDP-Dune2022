#pragma once

#include "common_utils.h"
#include "client_selectable.h"
#include "client_gamemap.h"
#include "client_gamehud.h"
#include "client_printer.h"
#include "protocol.h"
#include "mouse.h"
#include "protocol.h"
#include "client_camera.h"

#include "protocol.h"
#include "socket.h"

#include <iostream>

const int CAMERA_WIDTH = 1280;
const int CAMERA_HEIGHT = 720;

const int CAMERA_INITIAL_POS_X = 0;
const int CAMERA_INITIAL_POS_Y = 0;

class Player {

 private:
    Protocol protocol;
    Socket socket;
 public:
    size_t spice;
    size_t c_spice;
    size_t c_energy;
    int energy;
    double efficiency;
    std::map<int, std::unique_ptr<CSelectable>> elements;
    MouseHandler mouse;
    CPrinter printer;
    GameMap map;
    GameHud hud;
    SDL2pp::Window& game_window;
    SDL2pp::Renderer& game_renderer;
    Camera camera;
    bool is_holding_building;
    int building_held;

    Player(const char* host_name, const char* service_name,Camera & cam, SDL2pp::Window& window,SDL2pp::Renderer& renderer, size_t spice, size_t c_spice, int energy, size_t c_energy, std::vector<std::vector<std::string>>& map_data);

    void play();
    void renderMap();
    void renderHud();
    void addElement(unit_t type,State& desc);
    void addElement(building_t type,State& desc);
    void update(std::vector<State>& server_data, int spice, int energy);
    //   Socket/Protocol methods

    void print(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time);
    
    void addUnitButton(std::string& IMG_PATH, int& x, int& y, int& id);
    void addBuildButton(std::string& IMG_PATH, int& x, int& y, int& id);
    int checkBuild(int& x, int& y);
    int checkUnit(int& x, int& );
    bool checkHud(int& x, int& y);
    hud_button_t checkBtn(int& x, int& y);

};
