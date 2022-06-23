#pragma once

#include <iostream>
#include <map>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"

#define IMG_FORMAT ".png"

#define TRIKE_PATH "/unitsprites/trike"
#define HARVESTER_PATH "/unitsprites/harvester"
#define TANK_PATH "/unitsprites/tank"
#define INFANTRY_PATH "/unitsprites/infantry"
#define FREMEN_PATH "/unitsprites/fremen"
#define DEVASTATOR_PATH "/unitsprites/devastator"
#define SARDAUKAR_PATH "/unitsprites/sardaukar"

class RenderHandler {
    private:
    std::map<direction_t,const char *> directions = {
      {TOP,"/top"},
      {TOP_RIGHT,"/topright"},
      {RIGHT,"/right"},
      {BOTTOM_RIGHT,"/bottomright"},
      {BOTTOM,"/bottom"},
      {BOTTOM_LEFT,"/bottomleft"},
      {LEFT,"/left"},
      {TOP_LEFT,"/topleft"}
   };
   std::map<player_t,const char*> faction = {
       {ATREIDES,"/atreides"},
       {HARKONNEN,"/harkonnen"},
       {ORDOS,"/ordos"}
   };
   std::map<bool,const char*> special = {
       {true,"/special"},
       {false,""}
   };
    public:
    void renderMovable(
        SDL2pp::Texture & texture,
        SDL2pp::Renderer & renderer,
        player_t faction,
        unit_t type,
        direction_t dir,
        int pos_x,
        int pos_y, 
        int rel_pos_x,
        int rel_pos_y,
        int cam_pos_x,
        int cam_pos_y,
        int tile_size);
    void renderStatic(
        SDL2pp::Texture & texture,
        SDL2pp::Renderer & renderer,
        building_t type,
        int pos_x,
        int pos_y, 
        int cam_pos_x,
        int cam_pos_y,
        int tile_size
    );
};
