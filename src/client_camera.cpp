#include "client_camera.h"

Camera::Camera(int pos_x, int pos_y, int width, int height, int screen_width, int screen_height){
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->width = width;
    this->height = height;
    this->screen_width = screen_width;
    this->screen_height = screen_height;
}

void Camera::move(int x, int y){
    this->pos_x += x;
    this->pos_y += y;
    if ( this->pos_x < 0 )
        pos_x = 0;
    if ( this->pos_y < 0 )
        pos_y = 0;
    if ( this->pos_x > this->screen_width-this->width)
        this->pos_x = this->screen_width-this->width;
    if ( this->pos_y > this->screen_height-this->height)
        this->pos_y = this->screen_height-this->height;
}

