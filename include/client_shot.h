#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_renderhandler.h"

class Shot{
 private:
    int pixel_origin_pos_x;
    int pixel_origin_pos_y;
    int pixel_target_pos_x;
    int pixel_target_pos_y;
    int speed;
    unit_t unit;
    int current_pixel_pos_x;
    int current_pixel_pos_y;

    int shot_width;
    int shot_height;
    clock_t shot_delay;

    std::vector<int> x_positions;
    std::vector<int> y_positions;

 public:
    Shot(Position origin, Position target, int speed, unit_t unit);
    void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    void nextPosition();
    void getCurrentPosition(int& _current_pixel_pos_x, int& _current_pixel_pos_y);
    void update(bool& has_reached_its_target);
};
