#include "client_shot.h"

Shot::Shot(Position origin, Position target, int speed, unit_t unit){

    this->pixel_origin_pos_x = origin.x*TILE_SIZE;
    this->pixel_origin_pos_y = origin.y*TILE_SIZE;

    this->pixel_target_pos_x = target.x*TILE_SIZE;
    this->pixel_target_pos_y = target.y*TILE_SIZE;

    this->speed = speed;
    this->unit = unit;

    this->shot_width = 2;
    this->shot_height = 2;

    this->current_pixel_pos_x = this->pixel_origin_pos_x;
    this->current_pixel_pos_y = this->pixel_origin_pos_y;

    int x1 = this->pixel_origin_pos_x;
    int x2 = this->pixel_target_pos_x;

    int y1 = this->pixel_origin_pos_y;
    int y2 = this->pixel_target_pos_y;

    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps;

    if (abs(dx) > abs(dy))
    steps = abs(dx);
    else
    steps = abs(dy);

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
        } else {
            this->nextPosition();
            this->shot_delay = current_time;
        }
    }
    return;
}
