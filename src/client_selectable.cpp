#include "client_selectable.h"

CSelectable::CSelectable(std::string name,State & state){
    this->name = name;
    this->LP = state.LP;
    this->position = state.position;
    this->selected = false;
}

void CSelectable::update(State & new_state,SDL2pp::Renderer & renderer){
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
    this->render(renderer);
}

void CSelectable::render(SDL2pp::Renderer & renderer){
    std::cout << "Yo no debería estar aca" << std::endl;
}

CMovable::CMovable(std::string name,State & state,SDL2pp::Renderer & renderer, const std::string& path)
:
CSelectable(name,state),
texture(renderer,path)
{}

CStatic::CStatic(std::string name,State & state,SDL2pp::Renderer & renderer, const std::string& path)
:
CSelectable(name,state),
texture(renderer,path)
{}

void CMovable::render(SDL2pp::Renderer & renderer){
    if (this->name == "Trike"){
        std::cout << "Rendering a Trike" << std::endl;
        renderer.Copy(
            texture,						//	The sprite
            //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
            SDL2pp::Rect(2,2,32,32),		//	'cut' from the sprite (NullOpt for no cut)
            SDL2pp::Rect(this->position.x*TILE_SIZE,this->position.y*TILE_SIZE,16,16)				//	set to this part of the window		
        );
    }
    if (this->name == "Harvester"){
        std::cout << "Rendering a Harvester" << std::endl;
        renderer.Copy(
            texture,						//	The sprite
            //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
            SDL2pp::Rect(0,0,55,55),		//	'cut' from the sprite (NullOpt for no cut)
            SDL2pp::Rect(this->position.x*TILE_SIZE,this->position.y*TILE_SIZE,16,16)				//	set to this part of the window		
        );
    }
}

void CStatic::render(SDL2pp::Renderer & renderer){
    if (this->name == "Wind Trap"){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,65,82),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE,this->position.y*TILE_SIZE,AIR_TRAP_DIM_X*TILE_SIZE,AIR_TRAP_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->name == "Barrack"){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,75,80),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE,this->position.y*TILE_SIZE,BARRACK_DIM_X*TILE_SIZE,BARRACK_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->name == "Refinery"){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,110,82),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE,this->position.y*TILE_SIZE,REFINERY_DIM_X*TILE_SIZE,REFINERY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(100,12,110,82),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE,this->position.y*TILE_SIZE,REFINERY_DIM_X*TILE_SIZE,REFINERY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
}