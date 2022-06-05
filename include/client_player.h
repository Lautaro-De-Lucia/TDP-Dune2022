#include <vector>
#include <map>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_selectable.h"
#include "client_gamemap.h"


class CPlayer {
private:
    size_t spice;
    size_t c_spice;
    int energy;
    double efficiency;
    std::map<uint16_t,CSelectable> elements;
    GameMap map; 
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

public:
    CPlayer(size_t spice, size_t cspice, int energy, double efficiency, std::vector<std::vector<cell_t>> map_data);
    void addElement(unit_t type,State & desc);
    void addElement(building_t type,State & desc);
    void update(std::vector<State> & server_data);
};

