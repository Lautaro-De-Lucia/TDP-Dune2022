#include "client_shotshandler.h"



ShotsHandler::ShotsHandler() {

}

Position ShotsHandler::pixelToGP(int x, int y) {
    Position pos;
    int i = 0;
    while (true){
        if(x < i*TILE_SIZE*2){
            pos.x = i-1;
            break;
        }
        i++;
    }
    i = 0;
    while (true){
        if(y < i*TILE_SIZE*2){
            pos.y = i-1;
            break;
        }
        i++;
    }

    return pos;
}

void ShotsHandler::GPToPixel(Position pos, int& pixel_x, int& pixel_y) {

    pixel_x = 2 * pos.x * TILE_SIZE + TILE_SIZE;
    pixel_y = 2 * pos.y * TILE_SIZE + TILE_SIZE;
}

