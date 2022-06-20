#include "client_renderhandler.h"   
   
void RenderHandler::renderMovable(
        SDL2pp::Texture & texture,
        SDL2pp::Renderer & renderer,
        unit_t type,
        move_direction dir,
        int pos_x,
        int pos_y, 
        int cam_pos_x,
        int cam_pos_y,
        int tile_size)
        {
    int sprite_cut_x;
    int sprite_cut_y;
    int size;        
    switch(type){
        case TRIKE:
            sprite_cut_x =33;
            sprite_cut_y =30;
            size =32;
            break;
        case FREMEN:
            sprite_cut_x =33;
            sprite_cut_y =30;
            size =32;
            break;
        case INFANTRY:
            sprite_cut_x = 45;
            sprite_cut_y = 45;
            size =32;
            break;  
        case SARDAUKAR:
            sprite_cut_x =33;
            sprite_cut_y =30;
            size =32;
            break;  
        case TANK:
            sprite_cut_x =33;
            sprite_cut_y =30;
            size =32;
            break;    
        case DEVASTATOR:
            sprite_cut_x =33;
            sprite_cut_y =30;
            size =32;
            break;
        case HARVESTER:
            sprite_cut_x =52;
            sprite_cut_y =48;
            size =50;
            break;
        default:
            throw std::runtime_error("asd");
    }
    switch (dir){
        case TOP:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*0,sprite_cut_y*0,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break; 
        case TOP_RIGHT:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*4,sprite_cut_y*0,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break; 
        case RIGHT:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*0,sprite_cut_y*1,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break;             
        case BOTTOM_RIGHT:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*4,sprite_cut_y*1,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break;
        case BOTTOM:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*0,sprite_cut_y*2,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break;
        case BOTTOM_LEFT:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*4,sprite_cut_y*2,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break;
        case LEFT:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*0,sprite_cut_y*3,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break;  
        case TOP_LEFT:
            renderer.Copy(
                texture,						//	The sprite
                //	(x,y,w,h) -> top-left (x,y) coordinates, height & width
                SDL2pp::Rect(sprite_cut_x*4,sprite_cut_y*3,size,size),		//	'cut' from the sprite (NullOpt for no cut)
                SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,tile_size,tile_size)				//	set to this part of the window		
            );
            break;
        default:
            break;              
    }
}

void RenderHandler::renderStatic(
        SDL2pp::Texture & texture,
        SDL2pp::Renderer & renderer,
        building_t type,
        int pos_x,
        int pos_y, 
        int cam_pos_x,
        int cam_pos_y,
        int tile_size
    ){
    int size_x;
    int size_y;
    int dim_x;
    int dim_y;
    switch(type){
        case AIR_TRAP:
            size_x = 65;
            size_y = 80;
            dim_x = 3;
            dim_y = 3;
            break;
        case BARRACK:
            size_x = 75;
            size_y = 80;
            dim_x = 2;
            dim_y = 3;
            break;
        case REFINERY:
            size_x = 110;
            size_y = 82;
            dim_x = 3;
            dim_y = 3;
            break;
        default:
            throw std::runtime_error("asd");
    }
    renderer.Copy(
        texture,						
        SDL2pp::Rect(0,0,size_x,size_y),	
        SDL2pp::Rect(pos_x*tile_size-cam_pos_x,pos_y*tile_size-cam_pos_y,dim_x*tile_size,dim_y*tile_size)				//	set to this part of the window		
    );
}
