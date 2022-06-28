#include "client_shot.h"

Shot::Shot(Position origin, Position target, int speed, unit_t unit){

    this->pixel_origin_pos_x = origin.x*TILE_SIZE;
    this->pixel_origin_pos_y = origin.y*TILE_SIZE;

    this->pixel_target_pos_x = target.x*TILE_SIZE;
    this->pixel_target_pos_y = target.y*TILE_SIZE;

    this->speed = speed;
    this->unit = unit;

    this->m = (double) ((pixel_target_pos_y-pixel_origin_pos_y)/(pixel_target_pos_x-pixel_origin_pos_x));
    this->b = (double) (pixel_target_pos_y - (pixel_target_pos_x*(pixel_target_pos_y-pixel_origin_pos_y)/(pixel_target_pos_x-pixel_origin_pos_x))) ;

    this->shot_width = 4;
    this->shot_height = 4;

    this->current_pixel_pos_x = this->pixel_origin_pos_x;
    this->current_pixel_pos_y = this->pixel_origin_pos_y;

    this->shot_delay = clock();
}

void Shot::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){

    int xpos = this->current_pixel_pos_x-cam_pos_x;
    int ypos = this->current_pixel_pos_y-cam_pos_y;

    std::string unit_shot_path;
    unit_shot_path.append(DATA_PATH);

    switch (this->unit)
    {
    case TRIKE:
        unit_shot_path.append(TRIKE_SHOT_PATH);
        break;
    
    default:
        return;
        //break;
    }

    unit_shot_path.append(IMG_FORMAT);

    //std::cout << "unit_shot_path: " << unit_shot_path << std::endl;

    SDL2pp::Texture texture(renderer, unit_shot_path.c_str());
    texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(unit_shot_path.c_str()));
    renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,this->shot_width,this->shot_height));

    std::cout << "rendering shot on pixel position: (" << xpos << "," << ypos << ")" << std::endl;
}

void Shot::nextPosition(){

    int dx = this->pixel_target_pos_x - this->pixel_origin_pos_x;
    int dy = this->pixel_target_pos_y - this->pixel_origin_pos_y;

    //int next_pixel_pos_x = (int) this->current_pixel_pos_x + 2;
    //int next_pixel_pos_y = (int) floor(this->b + (this->m * next_pixel_pos_x));

    int next_pixel_pos_x = this->current_pixel_pos_x + 2;
    int next_pixel_pos_y = this->pixel_origin_pos_y + dy * (next_pixel_pos_x - this->pixel_origin_pos_x) / dx;

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
        this->nextPosition();
        if (this->current_pixel_pos_x > this->pixel_target_pos_x)
            has_reached_its_target = true;
        if (this->current_pixel_pos_y > this->pixel_target_pos_y)
            has_reached_its_target = true;
        this->shot_delay = current_time;
    }
    return;
}
