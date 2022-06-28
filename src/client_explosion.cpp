#include "client_explosion.h"

Explosion::Explosion(selectable_t type, Position pos) {

    this->pixel_pos_x = pos.x*TILE_SIZE - 16;
    this->pixel_pos_y = pos.y*TILE_SIZE - 16;

    this->type = type;

    this->explosion_width = 64;
    this->explosion_height = 64;

    this->speed = 10000;

    this->current_frame = 0;

    this->total_frames = 20;

    this->explosion_delay = clock();
}

void Explosion::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){

    std::cout << "rendering frame: " << this->current_frame << " of the explosion" << std::endl;

    int xpos = this->pixel_pos_x-cam_pos_x;
    int ypos = this->pixel_pos_y-cam_pos_y;

    std::string explosion_path;
    explosion_path.append(DATA_PATH);

    explosion_path.append(EXPLOSION_PATH);

    explosion_path.append(std::to_string(this->current_frame));

    explosion_path.append(IMG_FORMAT);

    SDL2pp::Texture texture(renderer, explosion_path.c_str());
    texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(explosion_path.c_str()));
    renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,this->explosion_width,this->explosion_height));

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
