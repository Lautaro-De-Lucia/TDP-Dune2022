#include "client_selectable.h"

CSelectable::CSelectable(std::string name,State & state,SDL2pp::Renderer & renderer,const std::string& lp_path)
:
lp_texture(renderer,lp_path)
{
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

void CStatic::update(State & new_state,SDL2pp::Renderer & renderer){
    if(this->name == "Wind Trap"){
        if(this->LP > (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*1))
            this->health = VERY_HIGH;
        if(this->LP > (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*2) && this->LP < (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*1))
            this->health = HIGH;
        if(this->LP > (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*3) && this->LP < (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*2))
            this->health = MEDIUM_HIGH;
        if(this->LP > (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*4) && this->LP < (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*3))
            this->health = MEDIUM;
        if(this->LP > (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*5) && this->LP < (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*4))
            this->health = MEDIUM_LOW;
        if(this->LP > (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*6) && this->LP < (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*5))
            this->health = LOW;
        if(this->LP > (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*7) && this->LP < (AIR_TRAP_LP -(AIR_TRAP_LP / 7)*6))
            this->health = VERY_LOW;                   
    }
    if(this->name == "Barrack"){
        if(this->LP > (BARRACK_LP -(BARRACK_LP / 7)*1))
            this->health = VERY_HIGH;
        if(this->LP > (BARRACK_LP -(BARRACK_LP / 7)*2) && this->LP < (BARRACK_LP -(BARRACK_LP / 7)*1))
            this->health = HIGH;
        if(this->LP > (BARRACK_LP -(BARRACK_LP / 7)*3) && this->LP < (BARRACK_LP -(BARRACK_LP / 7)*2))
            this->health = MEDIUM_HIGH;
        if(this->LP > (BARRACK_LP -(BARRACK_LP / 7)*4) && this->LP < (BARRACK_LP -(BARRACK_LP / 7)*3))
            this->health = MEDIUM;
        if(this->LP > (BARRACK_LP -(BARRACK_LP / 7)*5) && this->LP < (BARRACK_LP -(BARRACK_LP / 7)*4))
            this->health = MEDIUM_LOW;
        if(this->LP > (BARRACK_LP -(BARRACK_LP / 7)*6) && this->LP < (BARRACK_LP -(BARRACK_LP / 7)*5))
            this->health = LOW;
        if(this->LP > (BARRACK_LP -(BARRACK_LP / 7)*7) && this->LP < (BARRACK_LP -(BARRACK_LP / 7)*6))
            this->health = VERY_LOW;                   
    }
    if(this->name == "Refinery"){
        if(this->LP > (REFINERY_LP -(REFINERY_LP / 7)*1))
            this->health = VERY_HIGH;
        if(this->LP > (REFINERY_LP -(REFINERY_LP / 7)*2) && this->LP < (REFINERY_LP -(REFINERY_LP / 7)*1))
            this->health = HIGH;
        if(this->LP > (REFINERY_LP -(REFINERY_LP / 7)*3) && this->LP < (REFINERY_LP -(REFINERY_LP / 7)*2))
            this->health = MEDIUM_HIGH;
        if(this->LP > (REFINERY_LP -(REFINERY_LP / 7)*4) && this->LP < (REFINERY_LP -(REFINERY_LP / 7)*3))
            this->health = MEDIUM;
        if(this->LP > (REFINERY_LP -(REFINERY_LP / 7)*5) && this->LP < (REFINERY_LP -(REFINERY_LP / 7)*4))
            this->health = MEDIUM_LOW;
        if(this->LP > (REFINERY_LP -(REFINERY_LP / 7)*6) && this->LP < (REFINERY_LP -(REFINERY_LP / 7)*5))
            this->health = LOW;
        if(this->LP > (REFINERY_LP -(REFINERY_LP / 7)*7) && this->LP < (REFINERY_LP -(REFINERY_LP / 7)*6))
            this->health = VERY_LOW;                   
    }
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
    this->render(renderer);
}

void CMovable::update(State & new_state,SDL2pp::Renderer & renderer){
    if(this->name == "Trike"){
        if(this->LP > (TRIKE_LP -(TRIKE_LP / 7)*1))
            this->health = VERY_HIGH;
        if(this->LP > (TRIKE_LP -(TRIKE_LP / 7)*2) && this->LP < (TRIKE_LP -(TRIKE_LP / 7)*1))
            this->health = HIGH;
        if(this->LP > (TRIKE_LP -(TRIKE_LP / 7)*3) && this->LP < (TRIKE_LP -(TRIKE_LP / 7)*2))
            this->health = MEDIUM_HIGH;
        if(this->LP > (TRIKE_LP -(TRIKE_LP / 7)*4) && this->LP < (TRIKE_LP -(TRIKE_LP / 7)*3))
            this->health = MEDIUM;
        if(this->LP > (TRIKE_LP -(TRIKE_LP / 7)*5) && this->LP < (TRIKE_LP -(TRIKE_LP / 7)*4))
            this->health = MEDIUM_LOW;
        if(this->LP > (TRIKE_LP -(TRIKE_LP / 7)*6) && this->LP < (TRIKE_LP -(TRIKE_LP / 7)*5))
            this->health = LOW;
        if(this->LP > (TRIKE_LP -(TRIKE_LP / 7)*7) && this->LP < (TRIKE_LP -(TRIKE_LP / 7)*6))
            this->health = VERY_LOW;                   
    }
    if(this->name == "Harvester"){
        if(this->LP > (HARVESTER_LP -(HARVESTER_LP / 7)*1))
            this->health = VERY_HIGH;
        if(this->LP > (HARVESTER_LP -(HARVESTER_LP / 7)*2) && this->LP < (HARVESTER_LP -(HARVESTER_LP / 7)*1))
            this->health = HIGH;
        if(this->LP > (HARVESTER_LP -(HARVESTER_LP / 7)*3) && this->LP < (HARVESTER_LP -(HARVESTER_LP / 7)*2))
            this->health = MEDIUM_HIGH;
        if(this->LP > (HARVESTER_LP -(HARVESTER_LP / 7)*4) && this->LP < (HARVESTER_LP -(HARVESTER_LP / 7)*3))
            this->health = MEDIUM;
        if(this->LP > (HARVESTER_LP -(HARVESTER_LP / 7)*5) && this->LP < (HARVESTER_LP -(HARVESTER_LP / 7)*4))
            this->health = MEDIUM_LOW;
        if(this->LP > (HARVESTER_LP -(HARVESTER_LP / 7)*6) && this->LP < (HARVESTER_LP -(HARVESTER_LP / 7)*5))
            this->health = LOW;
        if(this->LP > (HARVESTER_LP -(HARVESTER_LP / 7)*7) && this->LP < (HARVESTER_LP -(HARVESTER_LP / 7)*6))
            this->health = VERY_LOW;                   
    }
    if (new_state.position.x == this->position.x && new_state.position.y > this->position.y){
        std::cout << "changing to BOTTOM" << std::endl;
        this->dir = BOTTOM;
    }
    if (new_state.position.x > this->position.x && new_state.position.y > this->position.y){
        std::cout << "changing to BOTTOM RIGHT" << std::endl;
        this->dir = BOTTOM_RIGHT;
    }
    if (new_state.position.x > this->position.x && new_state.position.y == this->position.y){
        std::cout << "changing to RIGHT" << std::endl;
        this->dir = RIGHT;
    }
    if (new_state.position.x > this->position.x && new_state.position.y < this->position.y){
        std::cout << "changing to TOP RIGHT" << std::endl; 
        this->dir = TOP_RIGHT;
    }
    if (new_state.position.x == this->position.x && new_state.position.y < this->position.y){
        std::cout << "changing to TOP" << std::endl; 
        this->dir = TOP;
    }
    if (new_state.position.x < this->position.x && new_state.position.y < this->position.y){
        std::cout << "changing to TOP LEFT" << std::endl;
        this->dir = TOP_LEFT; 
    }
    if (new_state.position.x < this->position.x && new_state.position.y == this->position.y){
        std::cout << "changing to LEFT" << std::endl;
        this->dir = LEFT;
    }
    if (new_state.position.x < this->position.x && new_state.position.y > this->position.y){
        std::cout << "changing to BOTTOM LEFT" << std::endl;
        this->dir = BOTTOM_LEFT;                    
    }
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
    this->render(renderer);
}

void CSelectable::render(SDL2pp::Renderer & renderer){
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
            lp_texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
            SDL2pp::Rect(30,20*this->health,100,20),		//	'cut' from the sprite (NullOpt for no cut)
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam.pos_x,(this->position.y-1)*TILE_SIZE-cam.pos_y,30,5) //	set to this part of the window		
        );    
}

void CStatic::render(SDL2pp::Renderer & renderer){
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
            lp_texture,						//	The sprite
                    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
            SDL2pp::Rect(30,20*this->health,100,20),		//	'cut' from the sprite (NullOpt for no cut)
            SDL2pp::Rect((this->position.x+0.5)*TILE_SIZE-cam.pos_x,(this->position.y)*TILE_SIZE-cam.pos_y,30,5) //	set to this part of the window		
        );
}