#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_explosion.h"
#include "client_audioplayer.h"

#define IMG_FORMAT ".png"

class ExplosionsHandler {
 private:
   std::vector<int> explosions_id;
   std::map<int, selectable_t> explosions_type;
   std::map<int, Position> explosions_position;
   std::vector<Explosion> explosions;
   AudioPlayer audio;
   SDL2pp::Renderer& renderer;

 public:
   ExplosionsHandler(SDL2pp::Renderer& renderer);
   ~ExplosionsHandler();
   void addExplosion(int id, selectable_t type, Position pos);
   void update();
   void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
};
