#include "client_gamehud.h"
#include <iostream>
#include <string>

#define HUD_IMG_PATH "../src/ui/resources/img/menu.png"

GameHud::GameHud(SDL2pp::Renderer & renderer) : texture(renderer, HUD_IMG_PATH) {}

void GameHud::render(SDL2pp::Renderer & renderer){
	int w = texture.GetWidth();
	int h = texture.GetHeight();
	int x = SCREEN_WIDTH - w;
	int y = SCREEN_HEIGHT - h;

	renderer.Copy(
		texture,						//	The sprite
		//	(x,y,w,h) -> top-left (x,y), height & width
		SDL2pp::NullOpt,		//	'cut' from the sprite (NullOpt for no cut)
		SDL2pp::Rect(x, y, w, h)				//	set to this part of the window		
	);
}