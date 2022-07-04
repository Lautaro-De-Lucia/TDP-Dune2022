#include "client_shot.h"

Shot::Shot(Position origin, Position target, int speed, unit_t unit){

    this->pixel_origin_pos_x = origin.x*TILE_SIZE;
    this->pixel_origin_pos_y = origin.y*TILE_SIZE;

    this->pixel_target_pos_x = target.x*TILE_SIZE + TILE_SIZE/2;
    this->pixel_target_pos_y = target.y*TILE_SIZE + TILE_SIZE/2;

    this->speed = speed;
    this->unit = unit;

    this->shot_width = 4;
    this->shot_height = 4;

    int dx = this->pixel_target_pos_x - this->pixel_origin_pos_x;
    int dy = this->pixel_target_pos_y - this->pixel_origin_pos_y;

    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    float x_increment = dx / (float) steps;
    float y_increment = dy / (float) steps;

    float current_pos_x = (float) this->pixel_origin_pos_x;
    float current_pos_y = (float) this->pixel_origin_pos_y;

    this->x_positions.push_back(round(current_pos_x));
    this->y_positions.push_back(round(current_pos_y));

    // all but last 8 pixels
    for (int i = 0; i < steps - 8; i++) {
        current_pos_x += x_increment;
        current_pos_y += y_increment;
        this->x_positions.push_back(round(current_pos_x));
        this->y_positions.push_back(round(current_pos_y));
    }

    // remove first 8 pixels
    for (size_t i = 0; i < 8; i++)
        this->nextPosition();

    this->current_pixel_pos_x = this->pixel_origin_pos_x;
    this->current_pixel_pos_y = this->pixel_origin_pos_y;

    this->shot_delay = clock();
}

void Shot::render(SDL2pp::Renderer& renderer, TextureHandler& textures, int cam_pos_x, int cam_pos_y){

    int xpos = this->current_pixel_pos_x-cam_pos_x;
    int ypos = this->current_pixel_pos_y-cam_pos_y;

    SDL2pp::Texture & shot = textures.getUnitShot(this->unit);
    renderer.Copy(
        shot,
        SDL2pp::NullOpt,
        SDL2pp::Rect(xpos,ypos,this->shot_width,this->shot_height)
    );
}

void Shot::nextPosition(){

    if (this->x_positions.size() == 0)
        return;

    int next_pixel_pos_x = this->x_positions.front();
    this->x_positions.erase(this->x_positions.begin());
    int next_pixel_pos_y = this->y_positions.front();
    this->y_positions.erase(this->y_positions.begin());

    this->current_pixel_pos_x = next_pixel_pos_x;
    this->current_pixel_pos_y = next_pixel_pos_y;
}

void Shot::getCurrentPosition(int& _current_pixel_pos_x, int& _current_pixel_pos_y) {

    _current_pixel_pos_x = this->current_pixel_pos_x;
    _current_pixel_pos_y = this->current_pixel_pos_y;
}

void Shot::update(bool& has_reached_its_target) {

    clock_t current_time = clock();

    has_reached_its_target = false;

    if (current_time - this->shot_delay > this->speed) {
        if (this->x_positions.size() == 0) {
            has_reached_its_target = true;
            return;
        } else {
            this->nextPosition();
        }
        if (this->x_positions.size() == 0) {
            has_reached_its_target = true;
            return;
        } else {
            this->nextPosition();
            this->shot_delay = current_time;
        }
    }
    return;
}
