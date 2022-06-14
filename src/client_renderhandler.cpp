#include "client_renderhandler.h"   
   
void RenderHandler::renderMovable(
        SDL2pp::Texture & texture,
        SDL2pp::Renderer & renderer,
        move_direction dir,
        int pos_x,
        int pos_y, 
        int cam_pos_x,
        int cam_pos_y,
        int tile_size,
        int sprite_cut_x, 
        int sprite_cut_y,
        int size){
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

void RenderHandler::renderStatic(SDL2pp::Texture & texture,int size_x, int size_y){

}