#include "client_selectable.h"

CSelectable::CSelectable(State& state, SDL2pp::Renderer& renderer, const std::string& lp_path)
:
lp_texture(renderer,lp_path)
{
    this->ID = state.ID;
    this->name = name;
    this->LP = state.LP;
    this->max_LP = state.LP;
    this->position = state.position;
    this->selected = false;
}

void CSelectable::update(State& new_state, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
    this->render(renderer, cam_pos_x, cam_pos_y);
}

void CStatic::update(State& new_state, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){

    //  Update health
    int hsprites = 7;
    for (size_t state = 1 ; state < hsprites ; state++){
        if (LP > (max_LP -(max_LP / 7)*state) && LP <= (max_LP -(max_LP / hsprites)*(state-1))){
            this->health = (health_t) state;
            break;
        }
    }
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;

    this->render(renderer, cam_pos_x, cam_pos_y);
}

void CMovable::update(State& new_state, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    
    int hsprites = 7;
    for (size_t state = 1 ; state < hsprites ; state++){
        if (LP > (max_LP -(max_LP / 7)*state) && LP <= (max_LP -(max_LP / hsprites)*(state-1))){
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
    this->render(renderer, cam_pos_x, cam_pos_y);
}

void CSelectable::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    //  Render Health
    if (this->selected == true)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y,30,5) 		
        );
}

int CSelectable::get_life_points() {

    return this->LP;
}

int CSelectable::getID() {

    return this->ID;
}



CMovable::CMovable(unit_t type, State& state, SDL2pp::Renderer& renderer, const std::string& lp_path , const std::string& path)
:
CSelectable(state,renderer,lp_path),
texture(renderer,path)
{
    this->dir = BOTTOM;
    this->type = type;     
}

CStatic::CStatic(building_t type, State& state, SDL2pp::Renderer& renderer, const std::string& lp_path, const std::string& path) 
:
CSelectable(state,renderer,lp_path),
texture(renderer,path)
{
    this->type = type;     
}

void CMovable::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){

    this->render_handler.renderMovable(this->texture,renderer,this->type,this->dir,this->position.x,this->position.y,cam_pos_x,cam_pos_y,TILE_SIZE);          
    
    if (this->selected == true)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y,30,5) 		
    );
}

void CStatic::render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    CSelectable::render(renderer, cam_pos_x, cam_pos_y);
    if (this->type == AIR_TRAP){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,65,80),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,AIR_TRAP_DIM_X*TILE_SIZE,AIR_TRAP_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->type == BARRACK){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,75,80),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,BARRACK_DIM_X*TILE_SIZE,BARRACK_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->type == REFINERY){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,110,82),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,REFINERY_DIM_X*TILE_SIZE,REFINERY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(100,12,110,82),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,REFINERY_DIM_X*TILE_SIZE,REFINERY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->selected == true)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y,30,5) 		
    );
}
