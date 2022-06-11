#include "client_gamehud.h"
#include <iostream>
#include <string>

#define HUD_IMG_PATH "../src/ui/resources/img/menu.png"
#define BARRACK_PATH "../src/ui/resources/img/barrack.png"
#define REFINERY_PATH "../src/ui/resources/img/refinery.png"
#define AIR_TRAP_PATH "../src/ui/resources/img/windtrap.png"
#define TRIKE_PATH "../src/ui/resources/img/trike.bmp"

GameHud::GameHud(SDL2pp::Renderer & renderer) : texture(renderer, HUD_IMG_PATH) {
	int w = texture.GetWidth();
	int h = texture.GetHeight();
	int x = SCREEN_WIDTH - w;
	int y = SCREEN_HEIGHT - h;
	dRect = {x, y, w, h};
	addBuildButton(renderer, BARRACK_PATH, SCREEN_WIDTH - 155, SCREEN_HEIGHT - 495, 2);
	addBuildButton(renderer, REFINERY_PATH, SCREEN_WIDTH - 155, SCREEN_HEIGHT - 435, 3);
	addBuildButton(renderer, AIR_TRAP_PATH, SCREEN_WIDTH - 155, SCREEN_HEIGHT - 375, 1);
	addUnitButton(renderer, TRIKE_PATH, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 495, 2);
}

void GameHud::render(SDL2pp::Renderer & renderer){
	renderer.Copy(
		texture,						//	The sprite
		//	(x,y,w,h) -> top-left (x,y), height & width
		SDL2pp::NullOpt,		//	'cut' from the sprite (NullOpt for no cut)
		dRect				//	set to this part of the window		
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
	int type = -1;
	for (size_t i = 0; i < build_buttons.size(); i++) {
		type = build_buttons[i].checkIntersection(x, y);
		if (type != -1) return type;
	} return type;
}

int GameHud::checkUnit(int &x, int &y) {
	int type = -1;
	for (size_t i = 0; i < unit_buttons.size(); i++) {
		type = unit_buttons[i].checkIntersection(x, y);
		if (type != -1) return type;
	} return type;
}

bool GameHud::clickOnHud(int &x, int &y){
	SDL2pp::Rect aux{x, y, 1, 1};
	return (dRect.Intersects(aux));
}