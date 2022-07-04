#pragma once

#include <memory>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

class CPrinter{
    private:
    size_t init_time;
    size_t end_time;
    int x;
    int y;
    int font_size;
    int message_size;
    std::vector<std::unique_ptr<SDL2pp::Texture>> textures;
    public:
    CPrinter();
    void print(std::unique_ptr<SDL2pp::Texture> texture,int x, int y, int font_size,int message_size);
    void timedPrint(std::unique_ptr<SDL2pp::Texture> texture,int x, int y, int font_size,int message_size, size_t time);
    void render(SDL2pp::Renderer & renderer);
    void renderTimed(SDL2pp::Renderer & renderer);
    void clearPrints();


};