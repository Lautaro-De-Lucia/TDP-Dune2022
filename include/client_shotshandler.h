#pragma once

#include <iostream>
#include <map>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"

#define IMG_FORMAT ".png"

class ShotsHandler {
 private:
    

 public:
    ShotsHandler();
    Position pixelToGP(int x, int y);
    void GPToPixel(Position pos, int& pixel_x, int& pixel_y);


};
