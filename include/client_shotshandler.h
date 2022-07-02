#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_shot.h"
#include "client_audioplayer.h"
#include "client_texturehandler.h"

#define IMG_FORMAT ".png"

#define TRIKE_BULLET_SPEED 1000
#define FREMEN_BULLET_SPEED 1000
#define INFANTRY_BULLET_SPEED 1000
#define SARDAUKAR_BULLET_SPEED 1000
#define TANK_BULLET_SPEED 1000
#define DEVASTATOR_BULLET_SPEED 1000

#define TRIKE_SHOT_FREQUENCY 3000000
#define FREMEN_SHOT_FREQUENCY 2000000
#define TANK_SHOT_FREQUENCY 3000000
#define DEVASTATOR_SHOT_FREQUENCY 5000000
#define INFANTRY_SHOT_FREQUENCY 1000000
#define SARDAUKAR_SHOT_FREQUENCY 2000000

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
    void render(SDL2pp::Renderer& renderer, TextureHandler& textures, int cam_pos_x, int cam_pos_y);
    void createShot(unit_t type, Position attacker_pos, Position target_pos);
};
