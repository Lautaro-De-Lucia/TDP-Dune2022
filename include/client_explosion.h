#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_texturehandler.h"

class Explosion{
 private:
   selectable_t type;
   int pixel_pos_x;
   int pixel_pos_y;

   int explosion_width;
   int explosion_height;
   int x_offset;
   int y_offset;
   clock_t explosion_delay;

   int speed;

   bool is_infantry;

   int current_frame;
   int total_frames;

   std::vector<int> remaining_frames;

 public:
   Explosion(selectable_t type, Position pos);
   void render(SDL2pp::Renderer& renderer, TextureHandler& textures, int cam_pos_x, int cam_pos_y);
   void nextFrame();
   void update(bool& has_ended);
};
