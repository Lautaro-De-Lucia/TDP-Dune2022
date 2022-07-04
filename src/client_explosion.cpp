#include "client_explosion.h"

Explosion::Explosion(selectable_t type, Position pos) {

    this->pixel_pos_x = pos.x*TILE_SIZE - 22;
    this->pixel_pos_y = pos.y*TILE_SIZE - 26;

    this->type = type;

    if (type == SEL_INFANTRY || type == SEL_FREMEN || type == SEL_SARDAUKAR)
        this->is_infantry = true;
    else
        this->is_infantry = false;

    switch (type)
    {
    case SEL_AIR_TRAP:
        this->explosion_width = 96;
        this->explosion_height = 96;
        this->x_offset = -8;
        this->y_offset = +2;
        break;
    case SEL_BARRACK:
        this->explosion_width = 96;
        this->explosion_height = 96;
        this->x_offset = -8;
        this->y_offset = +2;
        break;
    case SEL_HEAVY_FACTORY:
        this->explosion_width = 128;
        this->explosion_height = 128;
        this->x_offset = -8;
        this->y_offset = -8;
        break;
    case SEL_LIGHT_FACTORY:
        this->explosion_width = 96;
        this->explosion_height = 96;
        this->x_offset = -2;
        this->y_offset = -2;
        break;
    case SEL_PALACE:
        this->explosion_width = 96;
        this->explosion_height = 96;
        this->x_offset = -12;
        this->y_offset = -12;
        break;
    case SEL_REFINERY:
        this->explosion_width = 96;
        this->explosion_height = 96;
        this->x_offset = -8;
        this->y_offset = +2;
        break;
    case SEL_SILO:
        this->explosion_width = 50;
        this->explosion_height = 50;
        this->x_offset = +0;
        this->y_offset = +0;
        break;
    case SEL_CONSTRUCTION_YARD:
        this->explosion_width = 128;
        this->explosion_height = 128;
        this->x_offset = -12;
        this->y_offset = -12;
        break;
    default:
        this->explosion_width = 64;
        this->explosion_height = 64;
        this->x_offset = +0;
        this->y_offset = +0;
        break;
    }

    this->speed = 30000;

    this->current_frame = 0;

    this->total_frames = 20;

    this->explosion_delay = clock();
}

void Explosion::render(SDL2pp::Renderer& renderer, TextureHandler& textures, int cam_pos_x, int cam_pos_y){

    int xpos = this->pixel_pos_x-cam_pos_x+this->x_offset;
    int ypos = this->pixel_pos_y-cam_pos_y+this->y_offset;

    SDL2pp::Texture & explosion = textures.getExplosion(this->current_frame, this->is_infantry);
    renderer.Copy(
        explosion,
        SDL2pp::NullOpt,
        SDL2pp::Rect(xpos,ypos,this->explosion_width,this->explosion_height)
    );

    //std::cout << "rendering explosion on pixel position: (" << xpos << "," << ypos << ")" << std::endl;
}

void Explosion::nextFrame(){

    if (this->current_frame == this->total_frames - 1)
        return;

    int next_frame = this->current_frame + 1;

    this->current_frame = next_frame;
}

void Explosion::update(bool& has_ended) {

    clock_t current_time = clock();

    has_ended = false;

    if (current_time - this->explosion_delay > this->speed) {
        if (this->current_frame == this->total_frames - 1) {
            has_ended = true;
        } else {
            this->nextFrame();
            this->explosion_delay = current_time;
        }
    }
    return;
}
