#include "client_renderhandler.h"   
   
void RenderHandler::renderMovable(
        SDL2pp::Texture & texture,
        SDL2pp::Renderer & renderer,
        player_t faction,
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
    int xpos = pos_x*tile_size-cam_pos_x;
    int ypos = pos_y*tile_size-cam_pos_y;

    switch(type){
        case TRIKE:
            {
                std::string trike_path;
                trike_path.append(DATA_PATH TRIKE_PATH);
                trike_path.append(this->directions[dir]);
                trike_path.append(this->special[false]);
                trike_path.append(IMG_FORMAT);
                texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(trike_path.c_str()));
                renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,tile_size,tile_size));
            }
            break;
        case FREMEN:
            {
                std::string freemen_path;
                freemen_path.append(DATA_PATH FREMEN_PATH);
                freemen_path.append(this->directions[dir]);
                freemen_path.append(this->special[false]);
                freemen_path.append(IMG_FORMAT);
                texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(freemen_path.c_str()));
                renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,tile_size,tile_size));
            }
            break;
        case INFANTRY:
            {
                std::string infantry_path;
                infantry_path.append(DATA_PATH INFANTRY_PATH);
                infantry_path.append(this->directions[dir]);
                infantry_path.append(this->special[false]);
                infantry_path.append(IMG_FORMAT);
                texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(infantry_path.c_str()));
                renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,tile_size,tile_size));
            }
            break;
        case SARDAUKAR:
            {
                std::string sardaukar_path;
                sardaukar_path.append(DATA_PATH SARDAUKAR_PATH);
                sardaukar_path.append(this->directions[dir]);
                sardaukar_path.append(this->special[false]);
                sardaukar_path.append(IMG_FORMAT);
                texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(sardaukar_path.c_str()));
                renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,tile_size,tile_size));
            }
            break;
        case TANK:
            {
                std::string tank_path;
                tank_path.append(DATA_PATH TANK_PATH);
                tank_path.append(this->faction[faction]);
                tank_path.append(this->directions[dir]);
                tank_path.append(this->special[false]);
                tank_path.append(IMG_FORMAT);
                std::cout << "Rendering tank path: " << tank_path << std::endl;
                texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(tank_path.c_str()));
                renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,tile_size,tile_size));
            }
            break;
        case DEVASTATOR:
            {
                std::string devastator_path;
                devastator_path.append(DATA_PATH DEVASTATOR_PATH);
                devastator_path.append(this->directions[dir]);
                devastator_path.append(this->special[false]);
                devastator_path.append(IMG_FORMAT);
                texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(devastator_path.c_str()));
                renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,tile_size+5,tile_size+5));
            }
            break;
        case HARVESTER:
            {
                std::string harvester_path;
                harvester_path.append(DATA_PATH HARVESTER_PATH);
                harvester_path.append(this->directions[dir]);
                harvester_path.append(this->special[false]);
                harvester_path.append(IMG_FORMAT);
                texture.Update(SDL2pp::NullOpt,SDL2pp::Surface(harvester_path.c_str()));
                renderer.Copy(texture,SDL2pp::NullOpt,SDL2pp::Rect(xpos,ypos,tile_size,tile_size));
            }
            break;
        default:
            throw std::runtime_error("asd");         
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
