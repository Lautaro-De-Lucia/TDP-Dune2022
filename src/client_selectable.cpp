#include "client_selectable.h"

CSelectable::CSelectable(int id,int faction,int lp,int pos_x,int pos_y, SDL2pp::Renderer& renderer, const std::string& lp_path)
:
lp_texture(renderer,lp_path)
{
    this->ID = id;
    this->faction = faction;
    this->LP = lp;
    this->max_LP = lp;
    this->position = Position(pos_x,pos_y);
    this->selected = false;
}

void CSelectable::update(int lp,int pos_x,int pos_y,bool selected,bool special,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    int hsprites = 7;
    for (size_t state = 1 ; state <= hsprites ; state++){
        if (LP > (max_LP -(max_LP / 7)*state) && LP <= (max_LP -(max_LP / hsprites)*(state-1))){
            this->health = (health_t) state;
            break;
        }
    }
    this->LP = lp;
    this->position.x = pos_x;
    this->position.y = pos_y;
    this->selected = selected;
}

void CStatic::update(player_t player_faction, int lp,bool selected,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    CSelectable::update(lp,this->position.x,this->position.y,selected,false,renderer,cam_pos_x,cam_pos_y);
    this->render(player_faction, renderer, cam_pos_x, cam_pos_y);
}

void CMovable::update(player_t player_faction, int lp,int pos_x,int pos_y,bool selected,bool attacking,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    
    if (pos_x == this->position.x && pos_y > this->position.y)
        this->dir = BOTTOM;
    if (pos_x > this->position.x && pos_y > this->position.y)
        this->dir = BOTTOM_RIGHT;    
    if (pos_x > this->position.x && pos_y == this->position.y)
        this->dir = RIGHT;
    if (pos_x > this->position.x && pos_y < this->position.y)
        this->dir = TOP_RIGHT;
    if (pos_x == this->position.x && pos_y < this->position.y)
        this->dir = TOP;
    if (pos_x < this->position.x && pos_y < this->position.y)
        this->dir = TOP_LEFT; 
    if (pos_x < this->position.x && pos_y == this->position.y)
        this->dir = LEFT;
    if (pos_x < this->position.x && pos_y > this->position.y)
        this->dir = BOTTOM_LEFT;                    
        
    CSelectable::update(lp,pos_x,pos_y,selected,attacking,renderer,cam_pos_x,cam_pos_y);
    this->render(player_faction, renderer, cam_pos_x, cam_pos_y);
}

void CSelectable::render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    //  Render Health
    if (this->selected == true && this->faction == player_faction)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y,30,5) 		
        );
}

int CSelectable::get_life_points() {    return this->LP;}
int CSelectable::getID() {return this->ID;}

CMovable::CMovable(unit_t type,int id,int faction,int lp,int pos_x,int pos_y, SDL2pp::Renderer& renderer, const std::string& lp_path , const std::string& path)
:
CSelectable(id,faction,lp,pos_x,pos_y,renderer,lp_path),
texture(renderer,path)
{
    this->dir = BOTTOM;
    this->type = type; 
    this->special = special;    
}

CStatic::CStatic(building_t type, int id,int faction,int lp,int pos_x,int pos_y,SDL2pp::Renderer& renderer, const std::string& lp_path, const std::string& path) 
:
CSelectable(id,faction,lp,pos_x,pos_y,renderer,lp_path),
texture(renderer,path)
{
    this->type = type;     
}

void CMovable::render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){

    this->render_handler.renderMovable(this->texture,renderer,this->type,this->dir,this->position.x,this->position.y,cam_pos_x,cam_pos_y,TILE_SIZE);          
    
    if (this->selected == true && this->faction == player_faction)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y,30,5) 		
    );
}

void CStatic::render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    CSelectable::render(player_faction, renderer, cam_pos_x, cam_pos_y);
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
    if (this->selected == true  && this->faction == player_faction)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y,30,5) 		
    );
}
