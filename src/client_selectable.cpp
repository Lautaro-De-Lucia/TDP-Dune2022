#include "client_selectable.h"

CSelectable::CSelectable(std::string name,State & state,SDL2pp::Renderer & renderer,const std::string& lp_path)
:
lp_texture(renderer,lp_path)
{
    this->name = name;
    this->LP = state.LP;
    this->max_LP = state.LP;
    this->position = state.position;
    this->selected = false;
}

void CSelectable::update(State & new_state,SDL2pp::Renderer & renderer){
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
    this->render(renderer);
}

void CStatic::update(State & new_state,SDL2pp::Renderer & renderer){
    //  Update health
    int hsprites = 7;
    for (size_t state = 1 ; state < hsprites ; state++){
        if(LP > (max_LP -(max_LP / 7)*state) && LP <= (max_LP -(max_LP / hsprites)*(state-1))){
            this->health = (health_t) state;
            break;
        }
    }
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
    this->render(renderer);
}

void CMovable::update(State & new_state,SDL2pp::Renderer & renderer){
    
    int hsprites = 7;
    for (size_t state = 1 ; state < hsprites ; state++){
        if(LP > (max_LP -(max_LP / 7)*state) && LP <= (max_LP -(max_LP / hsprites)*(state-1))){
            this->health = (health_t) state;
            break;
        }
    }

    if (new_state.position.x == this->position.x && new_state.position.y > this->position.y)
        this->dir = BOTTOM;
    if (new_state.position.x > this->position.x && new_state.position.y > this->position.y)
        this->dir = BOTTOM_RIGHT;    
    if (new_state.position.x > this->position.x && new_state.position.y == this->position.y)
        this->dir = RIGHT;
    if (new_state.position.x > this->position.x && new_state.position.y < this->position.y)
        this->dir = TOP_RIGHT;
    if (new_state.position.x == this->position.x && new_state.position.y < this->position.y)
        this->dir = TOP;
    if (new_state.position.x < this->position.x && new_state.position.y < this->position.y)
        this->dir = TOP_LEFT; 
    if (new_state.position.x < this->position.x && new_state.position.y == this->position.y)
        this->dir = LEFT;
    if (new_state.position.x < this->position.x && new_state.position.y > this->position.y)
        this->dir = BOTTOM_LEFT;                    

    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
    this->render(renderer);
}

void CSelectable::render(SDL2pp::Renderer & renderer){
    //  Render Health
    if(this->selected == true)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam.pos_x,(this->position.y-0.5)*TILE_SIZE-cam.pos_y,30,5) 		
        );
}

CMovable::CMovable(std::string name,State & state,SDL2pp::Renderer & renderer,const std::string& lp_path ,const std::string& path)
:
CSelectable(name,state,renderer,lp_path),
texture(renderer,path)
{}

CStatic::CStatic(std::string name,State & state,SDL2pp::Renderer & renderer,const std::string& lp_path, const std::string& path)
:
CSelectable(name,state,renderer,lp_path),
texture(renderer,path)
{}

void CMovable::render(SDL2pp::Renderer & renderer){
    if (this->name == "Trike"){
        switch(this->dir){
            case TOP:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(2,2,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break; 
            case TOP_RIGHT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(33*4,0,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break; 
            case RIGHT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(0,30,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;             
            case BOTTOM_RIGHT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(33*4,30,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            case BOTTOM:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(0,30*2,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            case BOTTOM_LEFT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(33*4,30*2,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            case LEFT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(0,30*3,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;  
            case TOP_LEFT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(33*4,30*3,32,32),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            default:
                break;              
        }
    } 
    if (this->name == "Harvester"){
        switch(this->dir){
            case TOP:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(0,0,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break; 
            case TOP_RIGHT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(52*3,0,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break; 
            case RIGHT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(0,48,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;             
            case BOTTOM_RIGHT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(52*3,48,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            case BOTTOM:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(0,48*2,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            case BOTTOM_LEFT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(52*3,48*2,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            case LEFT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(0,48*3,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;  
            case TOP_LEFT:
                renderer.Copy(
                    texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                    SDL2pp::Rect(52*3,48*3,50,50),		//	'cut' from the sprite (NullOpt for no cut)
                    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,16,16)				//	set to this part of the window		
                );
                break;
            default:
                break;              
        }
    }
    if(this->selected == true)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam.pos_x,(this->position.y-0.5)*TILE_SIZE-cam.pos_y,30,5) 		
    );
}

void CStatic::render(SDL2pp::Renderer & renderer){
    CSelectable::render(renderer);
    if (this->name == "Wind Trap"){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,65,80),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,AIR_TRAP_DIM_X*TILE_SIZE,AIR_TRAP_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->name == "Barrack"){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,75,80),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,BARRACK_DIM_X*TILE_SIZE,BARRACK_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->name == "Refinery"){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,110,82),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,REFINERY_DIM_X*TILE_SIZE,REFINERY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(100,12,110,82),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam.pos_x,this->position.y*TILE_SIZE-cam.pos_y,REFINERY_DIM_X*TILE_SIZE,REFINERY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if(this->selected == true)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam.pos_x,(this->position.y-0.5)*TILE_SIZE-cam.pos_y,30,5) 		
    );
}