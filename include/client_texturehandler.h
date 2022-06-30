#pragma once

#include <map>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"

class TextureHandler {
    private:
    std::map<unit_t,std::map<player_t,std::map<direction_t,SDL2pp::Texture>>> unit_textures;
    std::map<building_t,std::map<player_t,SDL2pp::Texture>> building_textures;  
    public:
    TextureHandler(SDL2pp::Renderer & game_renderer);
    SDL2pp::Texture & getTexture(unit_t unit, player_t faction, direction_t direction);
    SDL2pp::Texture & getTexture(building_t building, player_t faction);
};
