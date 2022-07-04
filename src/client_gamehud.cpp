#include "client_gamehud.h"
#include <iostream>
#include <string>

#define HUD_IMG_PATH "../src/ui/resources/img/"
#define HUD_IMG_MENU_PATH "../src/ui/resources/img/newmenu.png"
/*
#define BARRACK_PATH "../src/ui/resources/img/barrack.bmp"
#define REFINERY_PATH "../src/ui/resources/img/refinery.bmp"
#define AIR_TRAP_PATH "../src/ui/resources/img/windtrap.bmp"
#define TRIKE_PATH "../src/ui/resources/img/trike.bmp"
#define HARVESTER_PATH "../src/ui/resources/img/harvester.bmp"
*/
#define ENERGY_PATH "../src/ui/resources/img/energy.jpg"

#define TOTAL_BUILDING_IMGS 8
#define TOTAL_UNIT_IMGS 8

GameHud::GameHud(player_t faction, SDL2pp::Renderer& renderer) : texture(renderer, HUD_IMG_MENU_PATH) {
	int w = texture.GetWidth();
	int h = texture.GetHeight();
	int x = SCREEN_WIDTH - w;
	int y = SCREEN_HEIGHT - h;
	dRect = {x, y, w, h};
	std::string faction_path;
	switch(faction){
		case HARKONNEN:
			faction_path.append("Harkonnen");
			break;
		case ATREIDES:
			faction_path.append("Atreides");
			break;	
		case ORDOS:
			faction_path.append("Ordos");
			break;	
	}
	
	std::vector<std::string> building_paths;
	std::vector<std::string> unit_paths;
	for(size_t i = 0 ; i < TOTAL_BUILDING_IMGS ; i++)
		building_paths.push_back(HUD_IMG_PATH);
	for(size_t i = 0 ; i < TOTAL_UNIT_IMGS ; i++)
		unit_paths.push_back(HUD_IMG_PATH);

	building_paths[BARRACK].append(faction_path.c_str()).append("/barrack.bmp");
	building_paths[REFINERY].append(faction_path.c_str()).append("/refinery.bmp");
	building_paths[AIR_TRAP].append(faction_path.c_str()).append("/wind-trap.bmp");
	building_paths[LIGHT_FACTORY].append(faction_path.c_str()).append("/light-factory.bmp");
	building_paths[HEAVY_FACTORY].append(faction_path.c_str()).append("/heavy-factory.bmp");
	building_paths[SILO].append(faction_path.c_str()).append("/silo.bmp");
	building_paths[PALACE].append(faction_path.c_str()).append("/palace.bmp");

	unit_paths[TRIKE].append(faction_path.c_str()).append("/trike.jpg");
	unit_paths[HARVESTER].append(faction_path.c_str()).append("/harvester.jpg");
	unit_paths[FREMEN].append(faction_path.c_str()).append("/fremen.bmp");
	unit_paths[INFANTRY].append(faction_path.c_str()).append("/light-infantry.bmp");
	unit_paths[SARDAUKAR].append(faction_path.c_str()).append("/sardaukar.bmp");
	unit_paths[TANK].append(faction_path.c_str()).append("/combat-tank.jpg");
	unit_paths[DEVASTATOR].append(faction_path.c_str()).append("/devastator.jpg");

	addBuildButton(renderer, building_paths[BARRACK].c_str(), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 495, BARRACK);
	addBuildButton(renderer, building_paths[REFINERY].c_str(), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 435, REFINERY);
	addBuildButton(renderer, building_paths[AIR_TRAP].c_str(), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 375, AIR_TRAP);
	addBuildButton(renderer, building_paths[LIGHT_FACTORY].c_str(), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 315, LIGHT_FACTORY);
	addBuildButton(renderer, building_paths[HEAVY_FACTORY].c_str(), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 255, HEAVY_FACTORY);
	addBuildButton(renderer, building_paths[SILO].c_str(), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 195, SILO);
	addBuildButton(renderer, building_paths[PALACE].c_str(), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 135, PALACE);

	addUnitButton(renderer, unit_paths[TRIKE].c_str(), SCREEN_WIDTH - 80, SCREEN_HEIGHT - 495, TRIKE);
	addUnitButton(renderer, unit_paths[HARVESTER].c_str(), SCREEN_WIDTH - 80, SCREEN_HEIGHT - 435, HARVESTER);
	addUnitButton(renderer, unit_paths[FREMEN].c_str(), SCREEN_WIDTH - 80, SCREEN_HEIGHT - 375, FREMEN);
	addUnitButton(renderer, unit_paths[INFANTRY].c_str(), SCREEN_WIDTH - 80, SCREEN_HEIGHT - 315, INFANTRY);
	addUnitButton(renderer, unit_paths[SARDAUKAR].c_str(), SCREEN_WIDTH - 80, SCREEN_HEIGHT - 255, SARDAUKAR);
	addUnitButton(renderer, unit_paths[TANK].c_str(), SCREEN_WIDTH - 80, SCREEN_HEIGHT - 195, TANK);
	addUnitButton(renderer, unit_paths[DEVASTATOR].c_str(), SCREEN_WIDTH - 80, SCREEN_HEIGHT - 135, DEVASTATOR);

}

SDL2pp::Texture & GameHud::getButtonInfo(hud_button_t button,int id,TextureHandler & textures){
	if(button == UNKNOWN_BTN)
		throw std::runtime_error("Unknown Button");
	if(button == BUILD_BTN) {
		switch(id){
			case BARRACK: return textures.getInfo(BARRACK);
			case REFINERY: return textures.getInfo(REFINERY);
			case AIR_TRAP: return textures.getInfo(AIR_TRAP);
			case LIGHT_FACTORY: return textures.getInfo(LIGHT_FACTORY);
			case HEAVY_FACTORY: return textures.getInfo(HEAVY_FACTORY);
			case SILO: return textures.getInfo(SILO);
			case PALACE: return textures.getInfo(PALACE);
		}
	} else if(button == UNIT_BTN) {
		switch(id){
			case FREMEN: return textures.getInfo(FREMEN);
			case INFANTRY: return textures.getInfo(INFANTRY);
			case SARDAUKAR: return textures.getInfo(SARDAUKAR);
			case TRIKE: return textures.getInfo(TRIKE);
			case TANK: return textures.getInfo(TANK);
			case DEVASTATOR: return textures.getInfo(DEVASTATOR);
			case HARVESTER: return textures.getInfo(HARVESTER);
		}
	}

	throw std::runtime_error("Unknown Button");
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

	SDL2pp::Texture energy_texture(renderer, ENERGY_PATH);
	
	if(this->energy <= 0)
		energy_texture.SetColorMod(255,0,0),
		renderer.Copy(energy_texture,SDL2pp::NullOpt,SDL2pp::Rect(1098,678-100/20,7,6+100/20));
	else
		renderer.Copy(energy_texture,SDL2pp::NullOpt,SDL2pp::Rect(1098,678-this->energy/20,7,6+this->energy/20));

	std::string spice_values;
	spice_values.append(std::to_string(this->spice));
	spice_values.append(" | ");
	spice_values.append(std::to_string(this->c_spice));

	//	RENDER SPICE
    SDL2pp::Font font(DATA_PATH FONT_IMPACT_PATH,10);
    SDL2pp::Surface surface = font.RenderText_Solid(spice_values,SDL_Color{255,255,0});
    SDL2pp::Texture spice_texture(renderer,surface);
	renderer.Copy(spice_texture,SDL2pp::NullOpt,SDL2pp::Rect(1145,665,80,30));
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

void GameHud::update(int spice, int c_spice, int energy){
	this->spice = spice;
	this->c_spice = c_spice;
	this->energy = energy;
}

void GameHud::setBuildButtonColor(double percentage, color_t color){
	for(Button & build_button : this->build_buttons)
		build_button.setTextureColor(percentage,color);
}

void GameHud::setUnitButtonColor(selectable_t type, double percentage, color_t color){
	if (percentage >= 1)
		percentage = 1;
	for(Button & unit_button : this->unit_buttons){
		if(unit_button.getID() == (int)type)
			unit_button.setTextureColor(percentage,color);
	}
}
