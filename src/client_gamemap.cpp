#include "client_gamemap.h"

void MapCell::render(SDL2pp::Renderer & renderer){
    renderer.Copy(
		this->texture,						//	The sprite
		//	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		SDL2pp::NullOpt,		//	'cut' from the sprite (NullOpt for no cut)
		SDL2pp::Rect(this->position.x*TILE_DIM,this->position.y*TILE_DIM,TILE_DIM,TILE_DIM)				//	set to this part of the window		
	);
}

GameMap::GameMap(std::vector<std::vector<cell_t>> & cells){
    
}