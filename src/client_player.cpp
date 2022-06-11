#include "client_player.h"

CPlayer::CPlayer(Camera& cam, SDL2pp::Window & window,SDL2pp::Renderer & renderer,size_t spice, size_t cspice, int energy, size_t c_energy, std::vector<std::vector<cell_t>> & map_data)
:
camera(cam),
game_window(window),
game_renderer(renderer),
map(renderer,map_data),
hud(renderer),
mouse(TILE_DIM,cam)
{
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    this->c_energy = c_energy;
    this->efficiency = efficiency;
}

void CPlayer::addElement(unit_t type,State & desc){
    //  Ahora estamos hablando de SPRITES
    //  Va a ser una imágen con la unidad en muchas posiciones
    //  Los cortes quedan a cargo de la unidad
    switch (type){
        case HARVESTER: 
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CMovable("Harvester",desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH HARVESTER_PATH))});
            break;
        case TRIKE:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CMovable("Trike",desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH TRIKE_PATH))});
            break;    
    }
}

void CPlayer::addElement(building_t type,State & desc){
    switch (type){
        case AIR_TRAP:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic("Wind Trap",desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH WIND_TRAP_PATH))});
            break;
        case BARRACK:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic("Barrack",desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH BARRACK_PATH))});
            break; 
        case REFINERY:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic("Refinery",desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH REFINERY_PATH))});
            break;       
    }
}

void CPlayer::update(std::vector<State>& server_data){

    this->game_renderer.Clear();
    this->renderMap();
    for (State data : server_data)
        (*(this->elements.at(data.ID))).update(data,this->game_renderer, this->camera.pos_x, this->camera.pos_y);

    this->renderHud();
    this->game_renderer.Present();
}

void CPlayer::renderMap(){
    this->game_renderer.SetScale(2,2);
    this->map.render(this->game_renderer, this->camera.pos_x, this->camera.pos_y);
}

void CPlayer::renderHud(){
    this->game_renderer.SetScale(1,1);
    this->hud.render(this->game_renderer);
}

void CPlayer::clientUpdate(std::vector<int> & mouse_event) {

//  GET MOUSE EVENT AND PUT ON QUEUE:

    SDL_Event event;
    mouse.getEvent(&event);
    SDL_PollEvent( &event );
    switch(event.type){
        case SDL_MOUSEBUTTONDOWN:
            if (mouse.leftClick()){
                mouse.click();
                mouse_event.push_back(MOUSE_LEFT_CLICK);
                Position clicked_pos = mouse.currentPosition();
                mouse_event.push_back(clicked_pos.x);
                mouse_event.push_back(clicked_pos.y);
            }
	        if (mouse.rightClick()){
                mouse.unclick();
                mouse_event.push_back(MOUSE_RIGHT_CLICK);
                Position clicked_pos = mouse.currentPosition();
                mouse_event.push_back(clicked_pos.x);
                mouse_event.push_back(clicked_pos.y);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            //	Si al soltarse no se movio de la posicion donde hizo click
            if (!(mouse.clickedPosition() == mouse.currentPosition())){
                Area selection = mouse.getSelection(mouse.clickedPosition(),mouse.currentPosition());
                mouse_event.push_back(MOUSE_SELECTION);
                mouse_event.push_back(selection.Xmin);
                mouse_event.push_back(selection.Xmax);
                mouse_event.push_back(selection.Ymin);
                mouse_event.push_back(selection.Ymax);
            }
            break;
        default:
            break;
    }


//  UPDATE CAMERA:
    SDL_PollEvent( &event );
    //User requests quit
    if( event.type == SDL_QUIT ) {
        this->game_window.Hide();
        return;
    }
    int x, y;
    SDL_GetMouseState( &x, &y );

    if (x < 80) {
        this->camera.move(-1,0);
        //sleepcp(x);
        //this->client_player.update(states);
    } else if (x > 1010 && x < 1090) {
        this->camera.move(1,0);
        //sleepcp(640-x);
        //this->client_player.update(states);
    }
    if (y < 60) {
        this->camera.move(0,-1);
        //sleepcp(y);
        //this->client_player.update(states);
    } else if (y > 660) {
        this->camera.move(0,1);
        //sleepcp(360-y);
        //this->client_player.update(states);
    }
}
