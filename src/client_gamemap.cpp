#include "client_gamemap.h"

void MapCell::setPosition(int pos_x, int pos_y){
	this->position.x = pos_x;
	this->position.y = pos_y;
}

void MapCell::setType(const char * new_type){
	this->type = new_type;
}

void MapCell::render(SDL2pp::Renderer& renderer,TextureHandler & textures, int cam_pos_x, int cam_pos_y){
	renderer.Copy(
		textures.getCell(this->type),						//	The sprite
		//	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		SDL2pp::NullOpt,		//	'cut' from the sprite (NullOpt for no cut)
		SDL2pp::Rect(this->position.x*TILE_DIM-cam_pos_x,this->position.y*TILE_DIM-cam_pos_y,TILE_DIM,TILE_DIM)				//	set to this part of the window		
	);
}

GameMap::GameMap(SDL2pp::Renderer& renderer,std::vector<std::vector<std::string>>& cell_data) {
	this->dim_x = cell_data.size();
	this->dim_y = cell_data[0].size();
	this->map_cells.resize(dim_x);
	for ( size_t i = 0; i < dim_x; ++i )
		map_cells[i].resize(dim_y);
    for (size_t j = 0 ; j < this->dim_y ; j++) {
        for (size_t i = 0 ; i < this->dim_x ; i++) {	
			map_cells[i][j].setPosition(i,j);
			map_cells[i][j].setType(cell_data[i][j].c_str());
        }
    }
}

void GameMap::render(SDL2pp::Renderer& renderer,TextureHandler & textures, int cam_pos_x, int cam_pos_y){
    for (size_t j = 0 ; j < this->dim_y ; j++) 
        for (size_t i = 0 ; i < this->dim_x ; i++)
			this->map_cells[i][j].render(renderer,textures,cam_pos_x,cam_pos_y);
}

void GameMap::updateCell(SDL2pp::Renderer& renderer,int pos_x, int pos_y,int spice) {
	if (spice == 0)
		map_cells[pos_x][pos_y].setType("s0");
	if (spice > 0 && spice < 200)
		map_cells[pos_x][pos_y].setType("s1");
	if (spice > 200 && spice < 500)
		map_cells[pos_x][pos_y].setType("s2");
	if (spice > 500 && spice < 1000)
		map_cells[pos_x][pos_y].setType("s3");
}
