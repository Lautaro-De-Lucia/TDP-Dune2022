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
        {RES_CREATE_BUILDING_FAILURE_TERRAIN, "You can't build here, dumbass"},
        {RES_SELECTED_BUILDING_IS_NOW_CREATOR, "The selected building is now a creator!"},
};

//  @TODO: Meter en un bloque try{} en el main
Player::Player(player_t faction, const char* host_name, const char* service_name,Camera & cam ,SDL2pp::Window& window,SDL2pp::Renderer& renderer,size_t spice, size_t cspice, int energy, size_t c_energy, std::vector<std::vector<std::string>>& map_data)
:
socket(host_name, service_name),
camera(cam),
game_window(window),
game_renderer(renderer),
map(renderer,map_data),
hud(faction,renderer),
mouse(TILE_DIM,cam)

{
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    this->c_energy = c_energy;
    this->efficiency = efficiency;
    this->is_holding_building = false;
    this->building_held = -1;
    this->left_click = false;
    this->right_click = false;
    this->selection = false;
    this->new_unit_available = true;
    this->faction = faction;
}

void Player::play(){

    this->handle_faction();

    bool game_has_started = false;
    auto base_time_instruction = clock();
    auto base_time_update = clock();
    std::vector<int> new_mouse_event;

    while (true) {

        //  loopeamos infinitamente
        //  Actualizamos la cámara
            
        SDL_Event event;
        SDL_PollEvent(&event);

        if( event.type == SDL_QUIT ) {
            new_mouse_event.clear();
            new_mouse_event.push_back(CLOSE);
            this->mouse_events.push(new_mouse_event);
            break;
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

        this->render();
            
        //  Obtenemos la instrucción del mouse
        mouse.getEvent(&event);
        Position current_pos = mouse.currentPosition();

        new_mouse_event.clear();

        switch(event.type) {
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

                            if (this->new_unit_available) {
                                new_mouse_event.push_back(CREATE_UNIT);
                                new_mouse_event.push_back(checkUnit(x,y));
                                new_mouse_event.push_back(current_pos.x);
                                new_mouse_event.push_back(current_pos.y);
                                this->is_holding_building = false;
                                this->building_held = -1;
                                this->mouse_events.push(new_mouse_event);
                                std::cout << "Queueing: " << "CREATE_UNIT" << std::endl;
                                this->new_unit_available = false;
                            }

                            break;
                        default:
                            this->is_holding_building = false;
                            this->building_held = -1;
                            break;
                        }
                    } else if (this->is_holding_building) {
                        new_mouse_event.push_back(CREATE_BUILDING);
                        new_mouse_event.push_back(this->building_held);
                        new_mouse_event.push_back(current_pos.x);
                        new_mouse_event.push_back(current_pos.y);
                        this->is_holding_building = false;
                        this->building_held = -1;
                        this->mouse_events.push(new_mouse_event);
                        std::cout << "Queueing: " << "CREATE_BUILDING" << std::endl;
                    } else {
                        new_mouse_event.push_back(MOUSE_LEFT_CLICK);
                        new_mouse_event.push_back(current_pos.x);
                        new_mouse_event.push_back(current_pos.y);
                        if (!this->left_click) {
                            this->mouse_events.push(new_mouse_event);
                            std::cout << "Queueing: " << "MOUSE_LEFT_CLICK" << std::endl;
                            this->left_click = true;
                            this->selection = false;
                        }
                    }
            }
                if (mouse.rightClick()){
                    mouse.unclick();
                    if(checkHud(x,y)) {
                    } else {
                        if (!this->right_click) {
                            new_mouse_event.push_back(MOUSE_RIGHT_CLICK);
                            new_mouse_event.push_back(current_pos.x);
                            new_mouse_event.push_back(current_pos.y);
                            this->mouse_events.push(new_mouse_event);
                            std::cout << "Queueing: " << "MOUSE_RIGHT_CLICK" << std::endl;
                            this->right_click = true;
                            this->selection = false;
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(checkHud(x,y)) {
                        this->new_unit_available = true;
                        break;
                }
                if (!(mouse.clickedPosition() == mouse.currentPosition())){
                    if(!this->selection) {
                        Area selection = mouse.getSelection(mouse.clickedPosition(),mouse.currentPosition());
                        new_mouse_event.push_back(MOUSE_SELECTION);
                        new_mouse_event.push_back(selection.Xmin);
                        new_mouse_event.push_back(selection.Xmax);
                        new_mouse_event.push_back(selection.Ymin);
                        new_mouse_event.push_back(selection.Ymax);
                        this->mouse_events.push(new_mouse_event);
                        std::cout << "Queueing: " << "MOUSE_SELECTION" << std::endl;
                        this->selection = true;
                    }

                    this->left_click = false;
                    this->right_click = false;

                }
                break;
            default:
                this->left_click = false;
                this->right_click = false;
                break;
        }

        auto current_time = clock();
	    auto frame_time_instruction = current_time - base_time_instruction;

        if (frame_time_instruction < GAME_SPEED && game_has_started)
            continue;

        game_has_started = true;

        base_time_instruction = current_time;

        std::vector<int> mouse_event;

        command_t command;

        if (this->mouse_events.size() > 0) {
            mouse_event = this->mouse_events.front();
            this->mouse_events.pop();
            command = (command_t)(mouse_event[0]);
        } else {
            command = IDLE;
        }

        //  La pasamos por socket
        this->protocol.send_command(command, this->socket);

        switch (command){
            case CREATE_UNIT:
                this->protocol.send_create_unit_request(mouse_event[1], this->socket);
                break;
            case CREATE_BUILDING:
                this->protocol.send_create_building_request(mouse_event[1], mouse_event[2],mouse_event[3],this->socket);
                break;
            case MOUSE_LEFT_CLICK:
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
        
        current_time = clock();
        auto frame_time_update = current_time - base_time_update;
        if (frame_time_update < 100 && game_has_started)
            continue;
        base_time_update = current_time;
        
        this->update();
    }
}

bool Player::contains(int ID) {
    for (auto& e : this->elements){
        if (e.first == ID)
            return true;
    }
    return false;
}

void Player::update() {
    //  Setup Variables
    int id,faction,lp,pos_x,pos_y,energy,spice; //  Values
    bool selected,harvesting,attacking; //  State flags
    int  toread,c;                      //  Helper variables
    //  Update values for player
    this->protocol.receive_player_state(spice,energy,this->socket);
    this->spice = spice;
    this->energy = energy;
    
    //  Update the board
    //  Get values in sand positions
    this->protocol.receive_sand_cells_size(toread,this->socket);
    for (c = 0 ; c < toread; c++){
        this->protocol.receive_sand_cell(pos_x,pos_y,spice,this->socket);
        this->map.updateCell(this->game_renderer,pos_x,pos_y,spice);
    }
    //  Render debris
        // int pos_x;
        // int pos_y;
        // int type;
        // for (size_t i = 0; i < toread; i++){ 
        // this->protocol.receive_sand_cell(pos_x,pos_y,spice,this->socket);
        // this->map_cells[pos_x][pos_y].setType(type);
        // }
    //  Receive elements
    //  Set updates to false
    for (size_t i = 0 ; i < this->updates.size(); i++)
        this->updates[i] = false;
    //  Get reads from server
    this->protocol.receive_selectables_to_read(toread, this->socket);
    //  Read each element
    std::cout << "Toread: " << toread << std::endl;
    for (size_t i = 0 ; i < toread ; i++){
        std::cout << "Asd: " << toread << std::endl;
        selectable_t type;
        this->protocol.receive_selectable_type(type,this->socket);
        switch(type){
            case SEL_TRIKE:
                this->protocol.receive_trike(id,faction,lp,pos_x,pos_y,selected,attacking,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,selected,attacking,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(TRIKE,id,faction,lp,pos_x,pos_y,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH TRIKE_PATH))});
                    this->updates.push_back(true);  
                }
                break;
            case SEL_HARVESTER:
                this->protocol.receive_harvester(id,faction,lp,pos_x,pos_y,selected,spice,harvesting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,selected,harvesting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(HARVESTER,id,faction,lp,pos_x,pos_y,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH HARVESTER_PATH))});
                    this->updates.push_back(true);  
                }
                break;
            case SEL_AIR_TRAP:
                this->protocol.receive_air_trap(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,selected,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(AIR_TRAP,id,faction,lp,pos_x,pos_y,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH WIND_TRAP_PATH))});
                    this->updates.push_back(true);  
                }     
                break;
            case SEL_REFINERY:
                this->protocol.receive_refinery(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,selected,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(REFINERY,id,faction,lp,pos_x,pos_y,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH REFINERY_PATH))});
                    this->updates.push_back(true);  
                } 
                break;
            case SEL_BARRACK:
                this->protocol.receive_barrack(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,selected,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(BARRACK,id,faction,lp,pos_x,pos_y,this->game_renderer,DATA_PATH LP_PATH,DATA_PATH BARRACK_PATH))});
                    this->updates.push_back(true);  
                }  
                break;     
        }
    }
    //  Destroy non-updated elements
    for (size_t i = 0 ; i < this->updates.size() ; i++)
        if(this->updates[i] == false)
            if(this->contains(i))
                this->elements.erase(i);     
}

/*
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
*/
void Player::render(){
    this->game_renderer.Clear();
    this->renderMap();
    this->printer.render(this->game_renderer);
    for (auto& e : this->elements)
        e.second->render(this->faction,this->game_renderer,this->camera.pos_x,this->camera.pos_y);
    this->hud.update(this->spice,this->energy);
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

bool Player::event_is_not_redundant(std::vector<int>& e) {

    if (e[0] == (int) MOUSE_SELECTION) {
        std::cout << "is mouse selection redundant?" << std::endl;
    }

    if (this->mouse_events.empty())
        return true;

    std::vector<int> last_event = this->mouse_events.back();

    if (e.size() != last_event.size())
        return true;

    size_t event_size = e.size();

    for (size_t i = 0; i < event_size; i++) {
        if (e[i] != last_event[i]) {
            return true;
        }
    }

    return false;
}

bool Player::handle_faction() {

    int _faction = 1;
    bool success = false;

    while (1) {
        //std::cout << "faction: "; // no flush needed
        //std::cin >> _faction;

        // HARKONNEN: 1, ATREIDES: 2, ORDOS: 3
        if (_faction < 1 || _faction > 3) {
            std::cout << "invalid faction, try again" << std::endl;
            continue;
        }

        this->protocol.send_faction_request((player_t) _faction, this->socket);
        this->protocol.receive_faction_request_response(success, this->socket);

        if (success) {
            break;
        }
        
        std::cout << "faction already in use, try again" << std::endl;

        _faction++;

        if (_faction > 3) {
            std::cout << "no factions available" << std::endl;
            return false;
        }
    }

    this->faction = (player_t) _faction;

    return true;
}
