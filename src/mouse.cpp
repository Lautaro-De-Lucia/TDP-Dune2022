#include "mouse.h"



MouseHandler::MouseHandler(size_t tile_dim,Camera & camera)
:
camera(camera)
{
    this->tile_dim = tile_dim;
}

void MouseHandler::getEvent(SDL_Event * mouse_event){
    this->mouse_event = mouse_event;
}

Position MouseHandler::currentPosition(){
    int x,y;
    Position pos;
    SDL_GetMouseState( &x, &y );
    int i = 0;
    while (true){
        if(x < i*tile_dim*2){
            pos.x = i-1;
            break;
        }
        i++;
    }
    i = 0;
    while (true){
        if(y < i*tile_dim*2){
            pos.y = i-1;
            break;
        }
        i++;
    }
    std::cout << "Current Mouse Position: " << pos << std::endl;
    std::cout << "Current Camera Position: (" << std::round(this->camera.pos_x/tile_dim) <<","<<std::round(this->camera.pos_y/tile_dim*2) <<")"<< std::endl;
    pos.x = pos.x + std::round(this->camera.pos_x/tile_dim);
    pos.y = pos.y + std::round(this->camera.pos_y/tile_dim);
    std::cout << "Mouse Position after summing camera: " << pos << std::endl;
    return pos;
}

Position MouseHandler::clickedPosition(){
    return this->mpos_on_click;
}

bool MouseHandler::leftClick(){
    return this->mouse_event->button.button == SDL_BUTTON_LEFT;
}

bool MouseHandler::rightClick(){
    return this->mouse_event->button.button == SDL_BUTTON_RIGHT;
}

void MouseHandler::click(){
    this->clicked = true;
    this->mpos_on_click = this->currentPosition();
}

void MouseHandler::unclick(){
    this->clicked = false;
    this->mpos_on_click = this->currentPosition();
}

Area MouseHandler::getSelection(Position init_pos, Position final_pos){
    Area selection(
        init_pos.x < final_pos.x ? init_pos.x : final_pos.x,
        init_pos.x >= final_pos.x ? init_pos.x : final_pos.x,
        init_pos.y < final_pos.y ? init_pos.y : final_pos.y,
        init_pos.y >= final_pos.y ? init_pos.y : final_pos.y
    );
    return selection;
}
