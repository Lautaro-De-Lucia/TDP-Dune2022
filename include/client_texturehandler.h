#pragma once

#include <map>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"

#define CMARK_PATH "/extrasprites/creator_mark.png"

class TextureHandler {
 private:
    std::map<unit_t,std::map<player_t,std::map<frame_t,std::map<direction_t,SDL2pp::Texture>>>>unit_textures;
    std::map<building_t,std::map<player_t,SDL2pp::Texture>> building_textures;
    std::map<unit_t,SDL2pp::Texture> unit_info_textures; 
    std::map<building_t,SDL2pp::Texture> building_info_textures;  
    std::map<std::string,SDL2pp::Texture> cell_textures;
    std::vector<SDL2pp::Texture> progress_bar_textures;
    std::map<player_t,std::map<unit_t,SDL2pp::Texture>> unit_imgs_textures;
    SDL2pp::Texture creator_mark;
    std::vector<SDL2pp::Texture> explosion_textures;
    std::vector<SDL2pp::Texture> blood_textures;
    std::map<unit_t,SDL2pp::Texture> unit_shot_textures;
    std::map<selectable_t,SDL2pp::Texture> corpse_textures;
    std::map<game_status_t,SDL2pp::Texture> game_status_textures;

 public:
    TextureHandler(SDL2pp::Renderer & game_renderer);
    SDL2pp::Texture & getTexture(unit_t unit, player_t faction,frame_t frame, direction_t direction);
    SDL2pp::Texture & getTexture(building_t building, player_t faction);
    SDL2pp::Texture & getInfo(unit_t unit);
    SDL2pp::Texture & getInfo(building_t building);
    SDL2pp::Texture & getCell(std::string & type);
    SDL2pp::Texture & getCreationProgress(int percentage);
    SDL2pp::Texture & getUnitIMG(player_t faction, unit_t unit);
    SDL2pp::Texture & getCreatorMark();
    SDL2pp::Texture & getExplosion(int frame, bool is_infantry);
    SDL2pp::Texture & getUnitShot(unit_t unit);
    SDL2pp::Texture & getCorpse(selectable_t unit);
    SDL2pp::Texture & getGameStatus(game_status_t game_status);
};
