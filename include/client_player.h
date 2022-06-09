#pragma once

#include <vector>
#include <map>
#include <unistd.h>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_selectable.h"
#include "client_gamemap.h"
#include "client_gamehud.h"

class CPlayer {
 private:
    size_t spice;
    size_t c_spice;
    size_t c_energy;
    int energy;
    double efficiency;
    std::map<int,std::unique_ptr<CSelectable>> elements;
    GameMap map;
    GameHud hud;
    SDL2pp::Window& game_window;
    SDL2pp::Renderer& game_renderer;
    Camera& camera;


 public:
    CPlayer(Camera& cam, SDL2pp::Window & window,SDL2pp::Renderer & renderer,size_t spice, size_t c_spice, int energy, size_t c_energy, std::vector<std::vector<cell_t>> & map_data);
    void renderMap();
    void renderHud();
    void addElement(unit_t type,State & desc);
    void addElement(building_t type,State & desc);
    void update(std::vector<State> & server_data);
    void updateCamera();
};
