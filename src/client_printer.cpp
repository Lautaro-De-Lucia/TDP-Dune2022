#include "client_printer.h"

CPrinter::CPrinter(){
    this->init_time = 0;
    this->end_time = 0;
    this->texture = nullptr;
    this-> x = 0;
    this-> y = 0;
    this->font_size = 0;
    this->message_size = 0;
}

void CPrinter::newPrint(std::unique_ptr<SDL2pp::Texture> texture,int x, int y, int font_size, int message_size, size_t time){
    this->init_time = SDL_GetTicks();
    this->end_time = init_time + time;
    this->x = x;
    this->y = y;
    this->texture = std::move(texture);
    this->font_size = font_size;
    this->message_size = message_size;
}

void CPrinter::render(SDL2pp::Renderer & renderer){
    if (SDL_GetTicks() < end_time){
        renderer.Copy(*(this->texture),
        SDL2pp::NullOpt,
        SDL2pp::Rect(this->x,this->y,this->font_size*(this->message_size/3),this->message_size));
    }
}

