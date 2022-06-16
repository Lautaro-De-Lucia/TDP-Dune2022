#include "client_gamemap.h"

MapCell::MapCell( SDL2pp::Renderer& renderer, const std::string& path,size_t pos_x, size_t pos_y)
:
position(pos_x,pos_y),
texture(renderer,path)
{
}

void MapCell::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
	renderer.Copy(
		texture,						//	The sprite
		//	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		SDL2pp::NullOpt,		//	'cut' from the sprite (NullOpt for no cut)
		SDL2pp::Rect(this->position.x*TILE_DIM-cam_pos_x,this->position.y*TILE_DIM-cam_pos_y,TILE_DIM,TILE_DIM)				//	set to this part of the window		
	);
}

GameMap::GameMap(SDL2pp::Renderer& renderer,std::vector<std::vector<std::string>>& cells) {
	size_t dim_x = cells.size();
	size_t dim_y = cells[0].size();
    for (size_t j = 0 ; j < dim_y ; j++) {
        for (size_t i = 0 ; i < dim_x ; i++) {
			map_cells.push_back(MapCell(renderer,cells[i][j].c_str(),i,j));	
        }
    }
}

void GameMap::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
	for (size_t i = 0 ; i < this->map_cells.size() ; i++)
		this->map_cells[i].render(renderer, cam_pos_x, cam_pos_y);
}
