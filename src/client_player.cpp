#include "client_player.h"
#include <unistd.h>

CPlayer::CPlayer(SDL2pp::Window & window,SDL2pp::Renderer & renderer,size_t spice, size_t cspice, int energy, size_t c_energy, std::vector<std::vector<cell_t>> & map_data)
:   
game_window(window),
game_renderer(renderer),
map(map_data)
{
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    this->c_energy = c_energy;
    this->efficiency = efficiency;
}

void CPlayer::addElement(unit_t type,State & desc){

}

void CPlayer::addElement(building_t type,State & desc){

}

void CPlayer::update(std::vector<State> & server_data){
    for (State data : server_data)
        this->elements.at(data.ID).update(data);
}