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
			map_cells[i][j] = std::make_unique<MapCell>(renderer,cell_paths[i][j].c_str(),i,j);
        }
    }
}

void GameMap::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    for (size_t j = 0 ; j < this->dim_y ; j++) 
        for (size_t i = 0 ; i < this->dim_x ; i++)
			this->map_cells[i][j]->render(renderer, cam_pos_x, cam_pos_y);
}

void GameMap::updateCells(SDL2pp::Renderer& renderer,std::vector<Position> sand_positions,std::vector<int> spice) {
	for(size_t i = 0; i < spice.size(); i++){
		if (spice[i] == 0)
			map_cells[sand_positions[i].x][sand_positions[i].y] = std::make_unique<MapCell>(renderer,"/home/lautaro/Documents/FACULTAD/TDP/Clonado2/TDP-Dune2022/data/mapsprites/s0",sand_positions[i].x,sand_positions[i].y);
		if (spice[i] < 0 && spice[i] < 200)
			map_cells[sand_positions[i].x][sand_positions[i].y] = std::make_unique<MapCell>(renderer,"/home/lautaro/Documents/FACULTAD/TDP/Clonado2/TDP-Dune2022/data/mapsprites/s1",sand_positions[i].x,sand_positions[i].y);
		if (spice[i] < 200 && spice[i] < 500)
			map_cells[sand_positions[i].x][sand_positions[i].y] = std::make_unique<MapCell>(renderer,"/home/lautaro/Documents/FACULTAD/TDP/Clonado2/TDP-Dune2022/data/mapsprites/s2",sand_positions[i].x,sand_positions[i].y);
		if (spice[i] < 500 && spice[i] < 1000)
			map_cells[sand_positions[i].x][sand_positions[i].y] = std::make_unique<MapCell>(renderer,"/home/lautaro/Documents/FACULTAD/TDP/Clonado2/TDP-Dune2022/data/mapsprites/s3",sand_positions[i].x,sand_positions[i].y);
	}
}
