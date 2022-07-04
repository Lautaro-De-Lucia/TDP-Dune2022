#include "client_printer.h"

CPrinter::CPrinter(){
    this->init_time = 0;
    this->end_time = 0;
    this-> x = 0;
    this-> y = 0;
    this->font_size = 0;
    this->message_size = 0;
}

void CPrinter::timedPrint(std::unique_ptr<SDL2pp::Texture> texture,int x, int y, int font_size, int message_size, size_t time){
    if(this->textures.size() > 4)
        this->textures.erase(this->textures.begin());
    this->init_time = SDL_GetTicks();
    this->end_time = init_time + time;
    this->x = x;
    this->y = y;
    this->textures.push_back(std::move(texture));
    this->font_size = font_size;
    this->message_size = message_size;
}

void CPrinter::print(std::unique_ptr<SDL2pp::Texture> texture,int x, int y, int font_size, int message_size){
    this->x = x;
    this->y = y;
    this->textures.push_back(std::move(texture));
    this->font_size = font_size;
    this->message_size = message_size;
}
void CPrinter::clearPrints(){
    this->textures.clear();
}


void CPrinter::renderTimed(SDL2pp::Renderer & renderer){
    if (SDL_GetTicks() <= end_time){
        for (size_t i = 0; i < this->textures.size() ; i++ )
            renderer.Copy(*(this->textures[i]),
            SDL2pp::NullOpt,
            SDL2pp::Rect(this->x,this->y+i*10,this->font_size*this->message_size/2,this->message_size));
    } else {
        this->textures.clear();
    }
}

void CPrinter::render(SDL2pp::Renderer & renderer){
    for (size_t i = 0; i < this->textures.size() ; i++ )
        renderer.Copy(*(this->textures[i]),
        SDL2pp::NullOpt,
        SDL2pp::Rect(this->x,this->y+i*10,this->font_size*this->message_size/2,this->message_size));
}
