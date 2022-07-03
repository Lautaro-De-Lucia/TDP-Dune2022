#include "client_selectable.h"

CSelectable::CSelectable(int id,int faction,int lp,int pos_x,int pos_y,int dim_x,int dim_y,SDL2pp::Renderer& renderer,TextureHandler & textures,const std::string& lp_path)
:
lp_texture(renderer,lp_path),
textures(textures)
{
    this->ID = id;
    this->faction = (player_t)faction;
    this->LP = lp;
    this->max_LP = lp;
    this->position = Position(pos_x,pos_y);
    this->dim_x = dim_x;
    this->dim_y = dim_y;
    this->selected = false;
}

void CSelectable::update(player_t player_faction, int lp,int pos_x,int pos_y,int speed,direction_t direction,bool moving,bool selected,bool special,bool waiting,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
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
    CSelectable::update(player_faction,lp,this->position.x,this->position.y,0,TOP,false,selected,false,false,renderer,cam_pos_x,cam_pos_y);
    this->render(player_faction, renderer, cam_pos_x, cam_pos_y);
}

void CMovable::update(player_t player_faction, int lp,int pos_x,int pos_y,int speed,direction_t direction,bool moving,bool selected,bool special,bool waiting,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){   

    CSelectable::update(player_faction,lp,pos_x,pos_y,speed,dir,moving,selected,special,waiting,renderer,cam_pos_x,cam_pos_y);

    if (this->dir != direction) {
        this->rel_pos_x = 0;
        this->rel_pos_y = 0;
    }

    this->dir = direction;
    this->sp = special;
    this->speed = speed;

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
}

int CSelectable::get_life_points() { return this->LP;}
int CSelectable::getID() {return this->ID;}

Position CSelectable::getPosition() {return this->position;}
player_t CSelectable::getFaction() {return this->faction;}
selectable_t CSelectable::getType() {std::cout << "i shouldnt be here" << std::endl; return (selectable_t) (-1);};

CMovable::CMovable(unit_t type,int id,int faction,int lp,int pos_x,int pos_y,int dim_x,int dim_y,SDL2pp::Renderer& renderer,TextureHandler & textures,const std::string& lp_path)
:
CSelectable(id,faction,lp,pos_x,pos_y,dim_x,dim_y,renderer,textures,lp_path)
{
    switch(faction){
        case ATREIDES:
            this->color.set(200,200,255);
            break;
        case HARKONNEN:
            this->color.set(255,200,200);
            break;
        case ORDOS:
            this->color.set(200,255,200);
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

CStatic::CStatic(building_t type, int id,int faction,int lp,int pos_x,int pos_y,int dim_x,int dim_y,SDL2pp::Renderer& renderer,TextureHandler & textures,const std::string& lp_path) 
:
CSelectable(id,faction,lp,pos_x,pos_y,dim_x,dim_y,renderer,textures,lp_path)
{
    switch(faction){
        case ATREIDES:
            this->color.set(200,200,255);
            break;
        case HARKONNEN:
            this->color.set(255,200,200);
            break;
        case ORDOS:
            this->color.set(200,255,200);
            break;
    }
    this->type = type;     
}

void CMovable::render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    //this->render_handler.renderMovable(this->texture,renderer,player_faction,this->type,this->dir,this->position.x,this->position.y,this->rel_pos_x,this->rel_pos_y,cam_pos_x,cam_pos_y,TILE_SIZE);          
    int xpos = this->position.x*TILE_SIZE-cam_pos_x+this->rel_pos_x;
    int ypos = this->position.y*TILE_SIZE-cam_pos_y+this->rel_pos_y;
    int xdim = this->dim_x*TILE_SIZE;
    int ydim = this->dim_y*TILE_SIZE;
    std::cout << "X offset: " << this->rel_pos_x <<" Y offset: "<<this->rel_pos_y<< std::endl;

    frame_t frame;
    
    if(abs(this->rel_pos_x) == 0 || abs(this->rel_pos_y) == 0)
        frame = ANIMATION_IDLE;    
    if(abs(this->rel_pos_x) == 2 || abs(this->rel_pos_y) == 2)
        frame = MOVING_1;
    if(abs(this->rel_pos_x) == 4 || abs(this->rel_pos_y) == 4)
        frame = MOVING_2;
    if(abs(this->rel_pos_x) == 6 || abs(this->rel_pos_y) == 6)
        frame = MOVING_1;
    if(abs(this->rel_pos_x) == 8 || abs(this->rel_pos_y) == 8)
        frame = ANIMATION_IDLE;
    if(abs(this->rel_pos_x) == 10 || abs(this->rel_pos_y) == 10)
        frame = MOVING_1;
    if(abs(this->rel_pos_x) == 12 || abs(this->rel_pos_y) == 12)
        frame = MOVING_2;
    if(abs(this->rel_pos_x) == 14 || abs(this->rel_pos_y) == 14)
        frame = MOVING_1;
    if(abs(this->rel_pos_x) == 16 || abs(this->rel_pos_y) == 16)
        frame = ANIMATION_IDLE;    

    if(this->sp == true && frame == ANIMATION_IDLE)
        frame = SPECIAL,
        std::cout << stringify(SPECIAL) << std::endl;

    renderer.Copy(
        this->textures.getTexture(this->type,(player_t)this->faction,animation,this->dir).SetColorMod(this->color.r,this->color.g,this->color.b),
        SDL2pp::NullOpt,
        SDL2pp::Rect(xpos,ypos,xdim,ydim)
    );

    if (this->selected == true && this->faction == player_faction)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect(xpos-TILE_SIZE/2,ypos-TILE_SIZE/2,30,5) 		
    );
}

void CStatic::render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y){
    // CSelectable::render(player_faction, renderer, cam_pos_x, cam_pos_y);
    int xpos = this->position.x*TILE_SIZE-cam_pos_x;
    int ypos = this->position.y*TILE_SIZE-cam_pos_y;
    int xdim = this->dim_x*TILE_SIZE;
    int ydim = this->dim_y*TILE_SIZE;
    renderer.Copy(
        this->textures.getTexture(this->type,(player_t)this->faction).SetColorMod(this->color.r,this->color.g,this->color.b),
        SDL2pp::NullOpt,
        SDL2pp::Rect(xpos,ypos,xdim,ydim)
    );
    if (this->selected == true && this->faction == player_faction)
        renderer.Copy(
            lp_texture,
            SDL2pp::Rect(30,20*(this->health-1),100,20),
            SDL2pp::Rect(xpos,ypos,30,5) 		
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
