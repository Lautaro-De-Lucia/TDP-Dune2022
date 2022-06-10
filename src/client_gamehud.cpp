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

	for (size_t i = 0; i < build_buttons.size(); i++) {
		build_buttons[i].render(renderer);
	}

	for (size_t i = 0; i < unit_buttons.size(); i++) {
		unit_buttons[i].render(renderer);
	}
}

void GameHud::addUnitButton(SDL2pp::Renderer &renderer, std::string IMG_PATH, int x, int y, int id) {
	this->unit_buttons.emplace_back(renderer, IMG_PATH, x, y, id);
}

void GameHud::addBuildButton(SDL2pp::Renderer &renderer, std::string IMG_PATH, int x, int y, int id) {
	this->build_buttons.emplace_back(renderer, IMG_PATH, x, y, id);
}

int GameHud::checkBuild(int &x, int &y) {
	int id = -1;
	for (size_t i = 0; i < build_buttons.size(); i++) {
		id = build_buttons[i].checkIntersection(x, y);
		if (id != -1) return id;
	} return id;
}

int GameHud::checkUnit(int &x, int &y) {
	int id = -1;
	for (size_t i = 0; i < unit_buttons.size(); i++) {
		id = unit_buttons[i].checkIntersection(x, y);
		if (id != -1) return id;
	} return id;
}

bool GameHud::clickOnHud(int &x, int &y){
	SDL2pp::Rect aux{x, y, 1, 1};
	return (dRect.Intersects(aux));
}