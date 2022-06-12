#include "client_gamehud_button.h"
#include <string>

#define DIM_BUTTON 50

Button::Button(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id) : texture(renderer, IMG_PATH),
    x(x), y(y), dRect{x, y, DIM_BUTTON, DIM_BUTTON}, id(id) {}

void Button::render(SDL2pp::Renderer& renderer){

	renderer.Copy(
		texture,						//	The sprite
		//	(x,y,w,h) -> top-left (x,y), height & width
		SDL2pp::NullOpt,		//	'cut' from the sprite (NullOpt for no cut)
		dRect				//	set to this part of the window		
	);
}

int Button::checkIntersection(int& x, int& y) {

	SDL2pp::Rect aux{x, y, 1, 1};
	if (dRect.Intersects(aux)) return id;
	return -1;
}
