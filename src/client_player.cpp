#include "client_player.h"

extern std::map<color_t,SDL_Color> colors;


CPlayer::CPlayer(Camera& cam, SDL2pp::Window& window,SDL2pp::Renderer& renderer,size_t spice, size_t cspice, int energy, size_t c_energy, std::vector<std::vector<cell_t>>& map_data)
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
    this->is_holding_building = false;
    this->building_held = -1;
}

void CPlayer::addElement(unit_t type,State& desc){
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

void CPlayer::addElement(building_t type, State& desc){
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

void CPlayer::update(std::vector<State>& server_data, int spice, int energy){
    this->game_renderer.Clear();
    this->renderMap();
    this->printer.render(this->game_renderer);
    for (State data : server_data)
        (*(this->elements.at(data.ID))).update(data,this->game_renderer, this->camera.pos_x, this->camera.pos_y);
    this->hud.update(spice,energy);
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

void CPlayer::clientUpdate(std::vector<int>& mouse_event) {

//  GET MOUSE EVENT AND PUT ON QUEUE:

    SDL_Event event;
    mouse.getEvent(&event);
    SDL_PollEvent(&event);
    Position current_pos = mouse.currentPosition();
    int pixel_x = -1;
    int pixel_y = -1;

    switch(event.type){
        case SDL_MOUSEBUTTONDOWN:
            if (mouse.leftClick()){
                mouse.click();
                SDL_GetMouseState(&pixel_x, &pixel_y);
                if (checkHud(pixel_x, pixel_y)) {
                    mouse.unclick();
                    switch (checkBtn(pixel_x, pixel_y))
                    {
                    case BUILD_BTN:
                        this->print(
                        "Place building on screen",
                        DATA_PATH FONT_IMPACT_PATH,
                        200,
                        300,
                        10,
                        colors[YELLOW],
                        1000);
                        this->is_holding_building = true;
                        this->building_held = checkBuild(pixel_x, pixel_y);
                        break;
                    case UNIT_BTN:
                        std::cout << "CREATING UNIT_BTN" << std::endl;
                        mouse_event.push_back(CREATE_UNIT);
                        mouse_event.push_back(checkUnit(pixel_x, pixel_y));
                        mouse_event.push_back(current_pos.x);
                        mouse_event.push_back(current_pos.y);
                        this->is_holding_building = false;
                        break;
                    default:
                        std::cout << "RELEASING BTN" << std::endl;
                        this->is_holding_building = false;
                        this->building_held = -1;
                        break;
                    }

                } else if (this->is_holding_building) {
                    mouse_event.push_back(CREATE_BUILDING);
                    mouse_event.push_back(this->building_held);
                    mouse_event.push_back(current_pos.x);
                    mouse_event.push_back(current_pos.y);
                    this->is_holding_building = false;
                    this->building_held = -1;
                } else {
                    mouse_event.push_back(MOUSE_LEFT_CLICK);
                    mouse_event.push_back(current_pos.x);
                    mouse_event.push_back(current_pos.y);
                    this->is_holding_building = false;
                    //this->building_held = -1;
                }
            }
	        if (mouse.rightClick()){
                mouse.unclick();
                if(checkHud(pixel_x, pixel_y)) {

                } else {
                    mouse_event.push_back(MOUSE_RIGHT_CLICK);
                    mouse_event.push_back(current_pos.x);
                    mouse_event.push_back(current_pos.y);
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(checkHud(pixel_x, pixel_y)) {
                    break;
            }
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
    SDL_PollEvent(&event);
    //User requests quit
    if( event.type == SDL_QUIT ) {
        this->game_window.Hide();
        return;
    }
    int x, y;
    SDL_GetMouseState(&x, &y);

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

void CPlayer::print(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time){
    SDL2pp::Font font(fontpath,size);
    SDL2pp::Surface surface = font.RenderText_Solid(toprint,color);
    std::unique_ptr texture = std::make_unique<SDL2pp::Texture>(SDL2pp::Texture(this->game_renderer,surface));
    this->printer.timedPrint(std::move(texture),x,y,toprint.size(),size,time);
}

void CPlayer::addUnitButton(std::string& IMG_PATH, int& x, int& y, int& id) {
    this->hud.addUnitButton(this->game_renderer, IMG_PATH, x, y, id);
}

void CPlayer::addBuildButton(std::string& IMG_PATH, int& x, int& y, int& id) {
    this->hud.addBuildButton(this->game_renderer, IMG_PATH, x, y, id);
}

int CPlayer::checkBuild(int& x, int& y) {
    return this->hud.checkBuild(x, y);
}

int CPlayer::checkUnit(int& x, int& y) {
    return this->hud.checkUnit(x, y);
}

bool CPlayer::checkHud(int& x, int& y) {
    return this->hud.clickOnHud(x, y);
}

hud_button_t CPlayer::checkBtn(int& x, int& y) {
    if (this->hud.checkBuild(x, y) != -1)
        return BUILD_BTN;
    if (this->hud.checkUnit(x, y) != -1)
        return UNIT_BTN;
    return UNKNOWN_BTN;
}
