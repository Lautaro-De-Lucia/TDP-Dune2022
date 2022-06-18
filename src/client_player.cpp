#include "client_player.h"

extern std::map<color_t,SDL_Color> colors;

std::map<response_t, std::string> usr_msg = {
        {RES_SUCCESS,""},
        {RES_CREATE_UNIT_SUCCESS, "Unit successfully created!"},
        {RES_CREATE_UNIT_FAILURE_SPICE, "Not enough spice"},
        {RES_CREATE_UNIT_FAILURE_CREATOR, "There's no creator for this unit"},
        {RES_CREATE_UNIT_FAILURE_SPACE, "Not enough space"},
        {RES_CREATE_BUILDING_SUCCESS, "Building successfully created!"},
        {RES_CREATE_BUILDING_FAILURE_SPICE, "Not enough spice"},
        {RES_CREATE_BUILDING_FAILURE_ENERGY, "Not enough energy"},
        {RES_CREATE_BUILDING_FAILURE_TERRAIN, "You can't build here, dumbass"}
};

//  @TODO: Meter en un bloque try{} en el main
Player::Player(const char* host_name, const char* service_name,Camera & cam ,SDL2pp::Window& window,SDL2pp::Renderer& renderer,size_t spice, size_t cspice, int energy, size_t c_energy, std::vector<std::vector<std::string>>& map_data)
:
socket(host_name, service_name),
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



void Player::play(){
    
    std::vector<int> mouse_event;
    while (true) {
    
    mouse_event.clear();
    //  loopeamos infinitamente
    //  Actualizamos la cámara
        
    SDL_Event event;
    SDL_PollEvent(&event);

    if( event.type == SDL_QUIT ) {
        exit(1);
    }
    
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (x < 80) 
        this->camera.move(-1,0);
    if (x > 1010 && x < 1090) 
        this->camera.move(1,0);
    if (y < 60) 
        this->camera.move(0,-1);
    if (y > 660) 
        this->camera.move(0,1);
        
    //  Obtenemos la instrucción del mouse
    mouse.getEvent(&event);
    Position current_pos = mouse.currentPosition();

    switch(event.type){
        case SDL_MOUSEBUTTONDOWN:
            if (mouse.leftClick()){
                mouse.click();
                SDL_GetMouseState(&x, &y);
                if (checkHud(x,y)) {
                    mouse.unclick();
                    switch (checkBtn(x, y))
                    {
                    case BUILD_BTN:
                        this->print("Place building on screen", DATA_PATH FONT_IMPACT_PATH, 200, 300, 10, colors[YELLOW], 1000);
                        this->is_holding_building = true;
                        this->building_held = checkBuild(x, y);
                        break;
                    case UNIT_BTN:
                        std::cout << "CREATING UNIT_BTN" << std::endl;
                        mouse_event.push_back(CREATE_UNIT);
                        mouse_event.push_back(checkUnit(x,y));
                        mouse_event.push_back(current_pos.x);
                        mouse_event.push_back(current_pos.y);
                        this->is_holding_building = false;
                        break;
                    default:
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
                if(checkHud(x,y)) {
                } else {
                    mouse_event.push_back(MOUSE_RIGHT_CLICK);
                    mouse_event.push_back(current_pos.x);
                    mouse_event.push_back(current_pos.y);
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(checkHud(x,y)) {
                    break;
            }
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
            mouse_event.push_back(IDLE);
            break;
        }
        //  La pasamos por socket
        command_t command = (command_t)(mouse_event[0]);
        std::cout << "Enviando el comando: " << command << std::endl;
        this->protocol.send_command(command, this->socket);

        switch (command){
            case CREATE_UNIT:
                this->protocol.send_create_unit_request(mouse_event[1], this->socket);
                break;
            case CREATE_BUILDING:
                this->protocol.send_create_building_request(mouse_event[1], mouse_event[2],mouse_event[3],this->socket);
                break;
            case MOUSE_LEFT_CLICK:
                //std::cout << "El cual representa un click izquierdo, y se envían las posiciones: " << std::endl;
                //std::cout << "pos_x: " << mouse_event[1] << std::endl;
                //std::cout << "pos_y: " << mouse_event[2] << std::endl;
                this->protocol.send_mouse_left_click(mouse_event[1], mouse_event[2], this->socket);
                break;
            case MOUSE_RIGHT_CLICK:
                this->protocol.send_mouse_right_click(mouse_event[1], mouse_event[2], this->socket);
                break;
            case MOUSE_SELECTION:
                this->protocol.send_mouse_selection(mouse_event[1], mouse_event[2], mouse_event[3], mouse_event[4], this->socket);
                break;
            case IDLE:
                break;
            default:
                break;
        }
        response_t res;
        this->protocol.receive_command_response(res,this->socket);
        if (res != RES_SUCCESS)
            this->print(usr_msg[res], DATA_PATH FONT_IMPACT_PATH, 200, 300, 10, res >= RESPONSE_FAILURE_OFFSET ? colors[GREEN] : colors[RED], 1000);
        this->update();
    }
}

void Player::addElement(unit_t type,State& desc) {
    //  Ahora estamos hablando de SPRITES
    //  Va a ser una imágen con la unidad en muchas posiciones
    //  Los cortes quedan a cargo de la unidad
    switch (type){
        case HARVESTER: 
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CMovable(HARVESTER,desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH HARVESTER_PATH))});
            break;
        case TRIKE:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CMovable(TRIKE,desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH TRIKE_PATH))});
            break;    
    }
}

void Player::addElement(building_t type, State& desc){
    switch (type){
        case AIR_TRAP:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic(AIR_TRAP,desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH WIND_TRAP_PATH))});
            break;
        case BARRACK:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic(BARRACK,desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH BARRACK_PATH))});
            break; 
        case REFINERY:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic(REFINERY,desc,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH REFINERY_PATH))});
            break;       
    }
}

void Player::update(){
    this->game_renderer.Clear();
    this->renderMap();
    this->printer.render(this->game_renderer);
    //for (State data : server_data)
    //    (*(this->elements.at(data.ID))).update(data,this->game_renderer, this->camera.pos_x, this->camera.pos_y);
    this->hud.update(1000,1000);
    this->renderHud();
    this->game_renderer.Present();
}

void Player::renderMap(){
    this->game_renderer.SetScale(2,2);
    this->map.render(this->game_renderer, this->camera.pos_x, this->camera.pos_y);
}

void Player::renderHud(){
    this->game_renderer.SetScale(1,1);
    this->hud.render(this->game_renderer);
}

void Player::print(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time){
    std::cout << "String to print: " << toprint << std::endl;
    SDL2pp::Font font(fontpath,size*10);
    SDL2pp::Surface surface = font.RenderText_Solid(toprint,color);
    std::unique_ptr texture = std::make_unique<SDL2pp::Texture>(SDL2pp::Texture(this->game_renderer,surface));
    this->printer.timedPrint(std::move(texture),x,y,toprint.size(),size,time);
}

void Player::addUnitButton(std::string& IMG_PATH, int& x, int& y, int& id) {
    this->hud.addUnitButton(this->game_renderer, IMG_PATH, x, y, id);
}

void Player::addBuildButton(std::string& IMG_PATH, int& x, int& y, int& id) {
    this->hud.addBuildButton(this->game_renderer, IMG_PATH, x, y, id);
}

int Player::checkBuild(int& x, int& y) {
    return this->hud.checkBuild(x, y);
}

int Player::checkUnit(int& x, int& y) {
    return this->hud.checkUnit(x, y);
}

bool Player::checkHud(int& x, int& y) {
    return this->hud.clickOnHud(x, y);
}

hud_button_t Player::checkBtn(int& x, int& y) {
    if (this->hud.checkBuild(x, y) != -1)
        return BUILD_BTN;
    if (this->hud.checkUnit(x, y) != -1)
        return UNIT_BTN;
    return UNKNOWN_BTN;
}


