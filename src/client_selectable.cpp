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

void CSelectable::update(player_t player_faction, int lp,int pos_x,int pos_y,direction_t direction,bool moving,bool selected,bool special,bool waiting,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
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

void CStatic::update(player_t player_faction, int lp,int pos_x,int pos_y,direction_t direction,bool moving,bool selected,bool special,bool waiting,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    CSelectable::update(player_faction,lp,this->position.x,this->position.y,TOP,false,selected,false,false,renderer,cam_pos_x,cam_pos_y);
    this->render(player_faction, renderer, cam_pos_x, cam_pos_y);
}

void CMovable::update(player_t player_faction, int lp,int pos_x,int pos_y,direction_t direction,bool moving,bool selected,bool special,bool waiting,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){   

    CSelectable::update(player_faction,lp,pos_x,pos_y,dir,moving,selected,special,waiting,renderer,cam_pos_x,cam_pos_y);

    if (this->dir != direction) {
        this->rel_pos_x = 0;
        this->rel_pos_y = 0;
    }

    this->dir = direction;
    this->sp = special;

    if(moving == true && waiting == false){
        if(this->dir == TOP)
            this->rel_pos_y-=this->speed;
        if(this->dir == TOP_RIGHT)
            this->rel_pos_y-=this->speed,
            this->rel_pos_x+=this->speed;
        if(this->dir == RIGHT)
            this->rel_pos_x+=this->speed;
        if(this->dir == BOTTOM_RIGHT)
            this->rel_pos_y+=this->speed,
            this->rel_pos_x+=this->speed;
        if(this->dir == BOTTOM)
            this->rel_pos_y+=this->speed;
        if(this->dir == BOTTOM_LEFT)
            this->rel_pos_y+=this->speed,
            this->rel_pos_x-=this->speed;
        if(this->dir == LEFT)
            this->rel_pos_x-=this->speed;
        if(this->dir == TOP_LEFT)
            this->rel_pos_y-=this->speed,
            this->rel_pos_x-=this->speed;
    } else {
        this->rel_pos_x = 0;
        this->rel_pos_y = 0;
    }

    if(std::abs(this->rel_pos_x) >= TILE_SIZE || std::abs(this->rel_pos_y) >= TILE_SIZE)
        this->rel_pos_x = 0,this->rel_pos_y = 0;

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

int CSelectable::get_life_points() { return this->LP;}
int CSelectable::getID() {return this->ID;}

Position CSelectable::getPosition() {std::cout << "i shouldnt be here" << std::endl;};
selectable_t CSelectable::getType() {std::cout << "i shouldnt be here" << std::endl;};

CMovable::CMovable(unit_t type,int id,int faction,int lp,int pos_x,int pos_y, SDL2pp::Renderer& renderer, const std::string& lp_path , const std::string& path)
:
CSelectable(id,faction,lp,pos_x,pos_y,renderer,lp_path),
texture(renderer,path)
{
    switch(faction){
        case ATREIDES:
            this->texture.SetColorMod(200,200,255);
            break;
        case HARKONNEN:
            this->texture.SetColorMod(255,200,200);
            break;
        case ORDOS:
            this->texture.SetColorMod(200,255,200);
            break;
    }
    switch(type){
        case TRIKE:
            this->speed = TRIKE_SPEED;
            break;
        case HARVESTER:
            this->speed = HARVESTER_SPEED;
            break;    
        case FREMEN:
            this->speed = FREMEN_SPEED;
            break;
        case INFANTRY:
            this->speed = INFANTRY_SPEED;
            break; 
        case SARDAUKAR:
            this->speed = SARDAUKAR_SPEED;
            break;
        case TANK:
            this->speed = TANK_SPEED;
            break; 
        case DEVASTATOR:
            this->speed = DEVASTATOR_SPEED;
            break;
    }
    this->dir = BOTTOM;
    this->type = type;
    this->rel_pos_x = 0;
    this->rel_pos_y = 0; 
    this->sp = false;   
}

CStatic::CStatic(building_t type, int id,int faction,int lp,int pos_x,int pos_y,SDL2pp::Renderer& renderer, const std::string& lp_path, const std::string& path) 
:
CSelectable(id,faction,lp,pos_x,pos_y,renderer,lp_path),
texture(renderer,path)
{
    switch(faction){
        case ATREIDES:
            this->texture.SetColorMod(200,200,255);
            break;
        case HARKONNEN:
            this->texture.SetColorMod(255,200,200);
            break;
        case ORDOS:
            this->texture.SetColorMod(200,255,200);
            break;
    }
    this->type = type;     
}

void CMovable::render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    this->render_handler.renderMovable(this->texture,renderer,player_faction,this->type,this->dir,this->position.x,this->position.y,this->rel_pos_x,this->rel_pos_y,cam_pos_x,cam_pos_y,TILE_SIZE);          
    if (this->selected == true && this->faction == player_faction)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x+this->rel_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y+this->rel_pos_y,30,5) 		
    );
}

void CStatic::render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    CSelectable::render(player_faction, renderer, cam_pos_x, cam_pos_y);
    if (this->type == CONSTRUCTION_YARD){
        if(this->faction == ATREIDES){
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,0,95,85),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,CONSTRUCTION_YARD_DIM_X*TILE_SIZE,CONSTRUCTION_YARD_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        }
        if(this->faction == HARKONNEN){
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,160,95,85),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,CONSTRUCTION_YARD_DIM_X*TILE_SIZE,CONSTRUCTION_YARD_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        }
        if(this->faction == ORDOS){
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,315,95,85),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,CONSTRUCTION_YARD_DIM_X*TILE_SIZE,CONSTRUCTION_YARD_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        }
    }
    if (this->type == AIR_TRAP){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,63,76),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,AIR_TRAP_DIM_X*TILE_SIZE,AIR_TRAP_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->type == BARRACK){
        if(this->faction == ATREIDES){
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,0,75,80),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,BARRACK_DIM_X*TILE_SIZE,BARRACK_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        }
        if(this->faction == HARKONNEN){
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,75,65,80),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,BARRACK_DIM_X*TILE_SIZE,BARRACK_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        }
        if(this->faction == ORDOS){
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,150,65,80),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,BARRACK_DIM_X*TILE_SIZE,BARRACK_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        }
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
    if (this->type == LIGHT_FACTORY){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,95,65),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,LIGHT_FACTORY_DIM_X*TILE_SIZE,LIGHT_FACTORY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(95,0,95,65),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x-2,this->position.y*TILE_SIZE-cam_pos_y,LIGHT_FACTORY_DIM_X*TILE_SIZE,LIGHT_FACTORY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->type == HEAVY_FACTORY){
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(0,0,95,90),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x-11,this->position.y*TILE_SIZE-cam_pos_y+4,HEAVY_FACTORY_DIM_X*TILE_SIZE,HEAVY_FACTORY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
        renderer.Copy(
		    texture,						//	The sprite
		    //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
		    SDL2pp::Rect(95,0,95,90),		//	'cut' from the sprite (NullOpt for no cut)
		    SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,HEAVY_FACTORY_DIM_X*TILE_SIZE,HEAVY_FACTORY_DIM_Y*TILE_SIZE)				//	set to this part of the window		
	    );
    }
    if (this->type == SILO){
        if(this->faction == ATREIDES)
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,0,35,30),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,SILO_DIM_X*TILE_SIZE,SILO_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        if(this->faction == ATREIDES)
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,30,35,30),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,SILO_DIM_X*TILE_SIZE,SILO_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        if(this->faction == ORDOS)
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,60,35,30),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,SILO_DIM_X*TILE_SIZE,SILO_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );

    }
    if (this->type == PALACE){
        if(this->faction == ATREIDES)
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,0,98,110),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,PALACE_DIM_X*TILE_SIZE,PALACE_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        if(this->faction == HARKONNEN)
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,115,98,110),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,PALACE_DIM_X*TILE_SIZE,PALACE_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
        if(this->faction == ORDOS)
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(0,225,98,110),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(this->position.x*TILE_SIZE-cam_pos_x,this->position.y*TILE_SIZE-cam_pos_y,PALACE_DIM_X*TILE_SIZE,PALACE_DIM_Y*TILE_SIZE)				//	set to this part of the window		
            );
    }
    if (this->selected == true  && this->faction == player_faction)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect((this->position.x-0.5)*TILE_SIZE-cam_pos_x,(this->position.y-0.5)*TILE_SIZE-cam_pos_y,30,5) 		
    );
}

Position CMovable::getPosition() {
    return this->position;
}

Position CStatic::getPosition() {
    return this->position;
}

selectable_t CMovable::getType() {
    return (selectable_t) this->type;
}

selectable_t CStatic::getType() {
   return (selectable_t) ((int) this->type + 7);
}
