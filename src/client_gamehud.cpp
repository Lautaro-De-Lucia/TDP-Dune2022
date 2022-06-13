#include "client_gamehud.h"
#include <iostream>
#include <string>

#define HUD_IMG_PATH "../src/ui/resources/img/newmenu.png"
#define BARRACK_PATH "../src/ui/resources/img/barrack.bmp"
#define REFINERY_PATH "../src/ui/resources/img/refinery.bmp"
#define ENERGY_PATH "../src/ui/resources/img/energy.jpg"
#define AIR_TRAP_PATH "../src/ui/resources/img/windtrap.bmp"
#define TRIKE_PATH "../src/ui/resources/img/trike.bmp"
#define HARVESTER_PATH "../src/ui/resources/img/harvester.bmp"


GameHud::GameHud(SDL2pp::Renderer& renderer) : texture(renderer, HUD_IMG_PATH) {
	int w = texture.GetWidth();
	int h = texture.GetHeight();
	int x = SCREEN_WIDTH - w;
	int y = SCREEN_HEIGHT - h;
	dRect = {x, y, w, h};
	addBuildButton(renderer, BARRACK_PATH, SCREEN_WIDTH - 155, SCREEN_HEIGHT - 495, BARRACK);
	addBuildButton(renderer, REFINERY_PATH, SCREEN_WIDTH - 155, SCREEN_HEIGHT - 435, REFINERY);
	addBuildButton(renderer, AIR_TRAP_PATH, SCREEN_WIDTH - 155, SCREEN_HEIGHT - 375, AIR_TRAP);
	addUnitButton(renderer, TRIKE_PATH, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 495, TRIKE);
	addUnitButton(renderer, HARVESTER_PATH, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 435, HARVESTER);
}

void GameHud::render(SDL2pp::Renderer& renderer){
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
	//	RENDER SPICE
    SDL2pp::Font font(DATA_PATH FONT_IMPACT_PATH,10);
    SDL2pp::Surface surface = font.RenderText_Solid(std::to_string(this->spice),SDL_Color{255,255,0});
    SDL2pp::Texture spice_texture(renderer,surface);
	renderer.Copy(spice_texture,SDL2pp::NullOpt,SDL2pp::Rect(1160,670,50,20));

	//	RENDER ENERGY
    SDL2pp::Texture energy_texture(renderer, ENERGY_PATH);
	renderer.Copy(energy_texture,SDL2pp::NullOpt,SDL2pp::Rect(1098,678-this->energy/20,7,6+this->energy/20));

}

void GameHud::renderSpice(){

}

void GameHud::renderEnergy(){

}
	

void GameHud::addUnitButton(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id) {
	this->unit_buttons.emplace_back(renderer, IMG_PATH, x, y, id);
}

void GameHud::addBuildButton(SDL2pp::Renderer& renderer, std::string IMG_PATH, int x, int y, int id) {
	this->build_buttons.emplace_back(renderer, IMG_PATH, x, y, id);
}

int GameHud::checkBuild(int& x, int& y) {
	int type = -1;
	for (size_t i = 0; i < build_buttons.size(); i++) {
		type = build_buttons[i].checkIntersection(x, y);
		if (type != -1) return type;
	} return type;
}

int GameHud::checkUnit(int& x, int& y) {
	int type = -1;
	for (size_t i = 0; i < unit_buttons.size(); i++) {
		type = unit_buttons[i].checkIntersection(x, y);
		if (type != -1) return type;
	} return type;
}

bool GameHud::clickOnHud(int& x, int& y){
	SDL2pp::Rect aux{x, y, 1, 1};
	return (dRect.Intersects(aux));
}

void GameHud::update(int spice, int energy){
	this->spice = spice;
	this->energy = energy;
}