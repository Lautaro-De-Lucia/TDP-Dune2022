#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_shot.h"
#include "client_audioplayer.h"

#define IMG_FORMAT ".png"


class ShotsHandler {
 private:
    std::vector<int> attackers_id;
    std::map<int, unit_t> attackers_type;
    std::map<int, Position> attackers_position;
    std::map<int, Position> attackers_target;
    std::map<int, clock_t> attackers_shot_cue;
    std::vector<Shot> shots;
    AudioPlayer audio;
    SDL2pp::Renderer& renderer;

 public:
    ShotsHandler(SDL2pp::Renderer& renderer);
    ~ShotsHandler();

    // updates if id is already an attacker, or adds a new attacker in case its not
    void updateAttacker(int id, unit_t type, Position attacker_pos, Position target_pos);
    void removeAttacker(int id);
    void update();
    void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
};
