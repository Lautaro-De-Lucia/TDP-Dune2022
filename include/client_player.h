#pragma once

#include <queue>
// ASD
#include "client_selectable.h"
#include "client_gamemap.h"
#include "client_gamehud.h"
#include "client_printer.h"
#include "client_mouse.h"
#include "client_camera.h"
#include "client_audioplayer.h"

#include "common_utils.h"
#include "common_protocol.h"
#include "common_socket.h"

#include <iostream>

const int CAMERA_WIDTH = 640;
const int CAMERA_HEIGHT = 360;

const int CAMERA_INITIAL_POS_X = 400;
const int CAMERA_INITIAL_POS_Y = 200;

const int GAME_SPEED = 20000;

#define DEF_TRIKE_PATH "/unitsprites/trike/top.png"
#define DEF_HARVESTER_PATH "/unitsprites/harvester/top.png"
#define DEF_FREMEN_PATH "/unitsprites/fremen/top.png"
#define DEF_INFANTRY_PATH "/unitsprites/infantry/top.png"
#define DEF_SARDAUKAR_PATH "/unitsprites/sardaukar/top.png"
#define DEF_TANK_PATH "/unitsprites/tank/top.png"
#define DEF_DEVASTATOR_PATH "/unitsprites/devastator/top.png"

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
    Camera & camera;
    bool is_holding_building;
    int building_held;
    std::queue<std::vector<int>> mouse_events;
    std::vector<bool> updates;
    bool left_click;
    bool right_click;
    bool selection;
    bool new_unit_available;
    player_t faction;
    AudioPlayer audio;

    std::vector<unit_t> attackers;
    std::vector<int> attackers_id;
    std::vector<clock_t> attackers_sound_cue;

    Player(player_t faction, const char* host_name, const char* service_name,Camera & cam, SDL2pp::Window& window,SDL2pp::Renderer& renderer, size_t spice, size_t c_spice, int energy, size_t c_energy, std::vector<std::vector<std::string>>& map_data);

    void play();
    void renderMap();
    void renderHud();
    void addElement(unit_t type,State& desc);
    void addElement(building_t type,State& desc);
    void render();
    void update();
    bool contains(int ID);

    //   Socket/Protocol methods

    void print(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time);
    
    void addUnitButton(std::string& IMG_PATH, int& x, int& y, int& id);
    void addBuildButton(std::string& IMG_PATH, int& x, int& y, int& id);
    int checkBuild(int& x, int& y);
    int checkUnit(int& x, int& );
    bool checkHud(int& x, int& y);
    hud_button_t checkBtn(int& x, int& y);

    void updateAttacker(unit_t unit, int id, bool attacking);
};
