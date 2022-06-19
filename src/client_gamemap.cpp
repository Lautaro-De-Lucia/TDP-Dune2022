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

GameMap::GameMap(SDL2pp::Renderer& renderer,std::vector<std::vector<std::string>>& cell_paths) {
	this->dim_x = cell_paths.size();
	this->dim_y = cell_paths[0].size();
	this->map_cells.resize(dim_x);
	for ( size_t i = 0; i < dim_x; ++i )
		map_cells[i].resize(dim_y);
    for (size_t j = 0 ; j < this->dim_y ; j++) {
        for (size_t i = 0 ; i < this->dim_x ; i++) {	
			map_cells[i][j] = std::unique_ptr<MapCell>(new MapCell(renderer,cell_paths[i][j].c_str(),i,j));
        }
    }
}

void GameMap::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    for (size_t j = 0 ; j < this->dim_y ; j++) 
        for (size_t i = 0 ; i < this->dim_x ; i++)
			this->map_cells[i][j]->render(renderer, cam_pos_x, cam_pos_y);
}

void GameMap::updateCell(SDL2pp::Renderer& renderer,int pos_x, int pos_y,int spice) {
	if (spice == 0)
		map_cells[pos_x][pos_y] = std::unique_ptr<MapCell>(new MapCell(renderer,DATA_PATH SAND_PATH_EMPTY,pos_x,pos_y));	
	if (spice > 0 && spice < 200)
		map_cells[pos_x][pos_y] = std::unique_ptr<MapCell>(new MapCell(renderer,DATA_PATH SAND_PATH_LITTLE,pos_x,pos_y));
	if (spice > 200 && spice < 500)
		map_cells[pos_x][pos_y] = std::unique_ptr<MapCell>(new MapCell(renderer,DATA_PATH SAND_PATH_SOME,pos_x,pos_y));
	if (spice > 500 && spice < 1000)
		map_cells[pos_x][pos_y] = std::unique_ptr<MapCell>(new MapCell(renderer,DATA_PATH SAND_PATH_FULL,pos_x,pos_y));
}
