#include "client_player.h"

extern std::map<color_t,SDL_Color> colors;

std::map<response_t, std::string> usr_msg = {
        {RES_SUCCESS,""},
        {RES_CREATE_UNIT_SUCCESS, "Unit successfully created!"},
        {RES_CREATE_UNIT_FAILURE_SPICE, "Not enough spice"},
        {RES_CREATE_UNIT_FAILURE_CREATOR, "There's no creator for this unit"},
        {RES_CREATE_UNIT_FAILURE_SPACE, "Not enough space"},
        {RES_CREATE_UNIT_FAILURE_SPECIAL, "Special Units may only be created when you have a Palace"},
        {RES_CREATE_BUILDING_SUCCESS, "Building successfully created!"},
        {RES_CREATE_BUILDING_FAILURE_SPICE, "Not enough spice"},
        {RES_CREATE_BUILDING_FAILURE_ENERGY, "Not enough energy"},
        {RES_CREATE_BUILDING_FAILURE_TERRAIN, "You can't build here"},
        {RES_SELECTED_BUILDING_IS_NOW_CREATOR, "The selected building is now a creator!"},
        {RES_WAIT_TO_BUILD,"You must wait to build new units"},
};

//  @TODO: Meter en un bloque try{} en el main
Player::Player(player_t faction, const char* host_name, const char* service_name,Camera & cam ,SDL2pp::Window& window,SDL2pp::Renderer& renderer,TextureHandler & textures,size_t spice, size_t cspice, int energy, size_t c_energy, std::vector<std::vector<std::string>>& map_data)
:
socket(host_name, service_name),
camera(cam),
game_window(window),
game_renderer(renderer),
map(renderer,map_data),
hud(faction,renderer),
mouse(TILE_SIZE,cam),
shotsHandler(renderer),
explosionsHandler(renderer),
textures(textures)
{
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    this->c_energy = c_energy;
    this->time_penalty = 0;
    this->construction_time = CONSTRUCTION_TIME;
    this->is_holding_building = false;
    this->building_held = -1;
    this->left_click = false;
    this->right_click = false;
    this->selection = false;
    this->w_key = false;
    this->a_key = false;
    this->s_key = false;
    this->d_key = false; 
    this->new_unit_available = true;
    this->faction = faction;
    this->game_has_started = false;

    this->base_alert_delay = clock();

    switch (faction)
    {
    case ATREIDES:
        this->base_position = Position(ATREIDES_INIT_POS_X, ATREIDES_INIT_POS_Y);
        break;
    case ORDOS:
        this->base_position = Position(ORDOS_INIT_POS_X, ORDOS_INIT_POS_Y);
        break;
    case HARKONNEN:
        this->base_position = Position(HARKONNEN_INIT_POS_X, HARKONNEN_INIT_POS_Y);
        break;
    
    default:
        this->base_position = Position(-1, -1);
        break;
    }

    this->creators.insert({BARRACK,-1});
    this->creators.insert({LIGHT_FACTORY,-1});
    this->creators.insert({HEAVY_FACTORY,-1});

    this->game_status = GAME_NEUTRAL;

    this->enemies = 0;
    this->destroyed_enemies = 0;

    this->renderWaitingText();

    //  Cargar Texturas
    this->protocol.receive_game_has_started(this->socket);
    std::cout << "Game Has Started" << std::endl;
    this->protocol.send_faction_request(this->faction, this->socket); 
    std::cout << "Faction Sent to The Server" << std::endl;

    this->audio.playStart(faction);

    this->base_time_instruction = clock();
    this->base_time_update = clock();
}

void Player::updateLoop(){
    while (true) {
        this->update();
    }
}

void Player::instructionLoop(){
    while (true) {

        this->game_has_started = true;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            this->protocol.send_command(CLOSE, this->socket);
            sleep(1);
            break;
        }
        this->handleEvent(event);
        
        if (this->usr_events.empty()){ 
            //std::cout << "T1: No instruction so far" << std::endl;
            continue;
        }
        std::cout << "T1: USR sent an instruction: " << std::endl;
        std::vector<int> usr_event = this->usr_events.front();
        this->usr_events.pop();
        command_t command = (command_t)(usr_event[0]);
        this->sendInstruction(command, usr_event);

        }      

        sleepms(20);  
    }
}

void Player::play(){
    std::thread server_updates(&Player::updateLoop, this);
    std::thread refresh_window(&Player::refresh, this);
    this->instructionLoop();
    server_updates.join();
}

bool Player::refresh(){
    while(true){
        sleepms(30);
        this->render();
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
    this->handleResponse();
    std::lock_guard<std::mutex> locker(this->lock);
    this->updatePlayerState();
    this->updateUtils();
    this->updateHud();
    this->updateBoard();
    this->updateSelectables();
    this->updateCreationData();  
}

void Player::updateCreationData(){
    //  Receive creation data
    this->creation_data.clear(); 
    int size,creator_ID,unit,current_time,total_time;
    this->protocol.receive_creation_data_size(size, this->socket);
    for (size_t i = 0; i < size ; i++)
        this->protocol.receive_creation_data(creator_ID,unit,current_time,total_time,this->socket),
        this->creation_data.push_back(creation_t(creator_ID,(unit_t)unit,current_time,total_time));
    //  Receive creators
    int barrack_id = -1;
    int light_factory_id = -1;
    int heavy_factory_id = -1;

    this->protocol.receive_creators(barrack_id, light_factory_id, heavy_factory_id, this->socket);

    this->creators[BARRACK] = barrack_id;
    this->creators[LIGHT_FACTORY] = light_factory_id;
    this->creators[HEAVY_FACTORY] = heavy_factory_id;
}

void Player::renderCreationData() {    
    for(creation_t & c : this->creation_data){
        //std::cout << "Creator ID: " << c.creator_ID << std::endl;
        std::unique_ptr<CSelectable> & element = this->elements[c.creator_ID];
        Position pos = element->getPosition();
        switch(element->getType()){
            case SEL_BARRACK:
                this->hud.setUnitButtonColor(SEL_INFANTRY,((double)c.current_time/c.total_time),faction_colours[this->faction]);
                this->hud.setUnitButtonColor(SEL_FREMEN,((double)c.current_time/c.total_time),faction_colours[this->faction]);
                this->hud.setUnitButtonColor(SEL_SARDAUKAR,((double)c.current_time/c.total_time),faction_colours[this->faction]);
                break;
            case SEL_LIGHT_FACTORY:
                this->hud.setUnitButtonColor(SEL_TRIKE,((double)c.current_time/c.total_time),faction_colours[this->faction]);
                this->hud.setUnitButtonColor(SEL_HARVESTER,((double)c.current_time/c.total_time),faction_colours[this->faction]);
                break;
            case SEL_HEAVY_FACTORY:
                this->hud.setUnitButtonColor(SEL_TANK,((double)c.current_time/c.total_time),faction_colours[this->faction]);
                this->hud.setUnitButtonColor(SEL_DEVASTATOR,((double)c.current_time/c.total_time),faction_colours[this->faction]);
                break;
        }
        SDL2pp::Texture & progress = this->textures.getCreationProgress(std::round(((double)c.current_time/c.total_time)*100));
        this->game_renderer.Copy(
            progress,
            SDL2pp::NullOpt,
            SDL2pp::Rect(pos.x*TILE_SIZE*2-this->camera.pos_x*2,pos.y*TILE_SIZE*2-this->camera.pos_y*2,10,80)
        );
        SDL2pp::Texture & unit_being_created = this->textures.getUnitIMG(this->elements[c.creator_ID]->getFaction(),c.unit_being_created);
        this->game_renderer.Copy(
            unit_being_created,
            SDL2pp::NullOpt,
            SDL2pp::Rect(pos.x*TILE_SIZE*2-this->camera.pos_x*2,(pos.y-1)*TILE_SIZE*2-this->camera.pos_y*2,30,30)
        );
    }
}

void Player::renderCreators() {

    std::vector<int> creators_to_render;

    if (this->creators[BARRACK] != -1)
        creators_to_render.push_back(this->creators[BARRACK]);

    if (this->creators[LIGHT_FACTORY] != -1)
        creators_to_render.push_back(this->creators[LIGHT_FACTORY]);

    if (this->creators[HEAVY_FACTORY] != -1)
        creators_to_render.push_back(this->creators[HEAVY_FACTORY]);

    for (int id : creators_to_render){
        Position pos = this->elements.at(id)->getPosition();
        selectable_t type = this->elements.at(id)->getType();
        SDL2pp::Texture & creator_mark = this->textures.getCreatorMark();
        int x_offset = -1;
        int y_offset = -1;
        if (type == SEL_BARRACK) {
            x_offset = -12;
            y_offset = -4;
        } else if (type == SEL_LIGHT_FACTORY) {
            x_offset = +0;
            y_offset = +0;
        } else if (type == SEL_HEAVY_FACTORY) {
            x_offset = +13;
            y_offset = -7;
        }
        this->game_renderer.Copy(
            creator_mark,
            SDL2pp::NullOpt,
            SDL2pp::Rect((pos.x+1)*TILE_SIZE*2-this->camera.pos_x*2+x_offset,(pos.y-1)*TILE_SIZE*2-this->camera.pos_y*2+y_offset,20,30)
        );
    }
}

void Player::renderWaitingText() {

    AudioPlayer audio;
    audio.play(WAITING_MUSIC);

    this->game_renderer.Clear();
    this->game_renderer.SetScale(2,2);
    std::string waiting_text = "WAITING FOR OTHER PLAYERS...";
    SDL2pp::Font waiting_font(DATA_PATH FONT_IMPACTED_PATH, 20);
    SDL2pp::Texture text_sprite_waiting(this->game_renderer, waiting_font.RenderText_Blended(waiting_text, SDL_Color{255, 255, 255, 255}));
    this->game_renderer.Copy(text_sprite_waiting, SDL2pp::NullOpt, SDL2pp::Rect(SCREEN_WIDTH/6, SCREEN_HEIGHT/6, text_sprite_waiting.GetWidth(), text_sprite_waiting.GetHeight()));
    std::string house_text = "You chose house: ";
    switch (this->faction)
    {
    case HARKONNEN: house_text += "Harkonnen"; break;
    case ATREIDES: house_text += "Atreides"; break;
    case ORDOS: house_text += "Ordos"; break;
    default: house_text += "Invalid house"; break;
    }
    SDL2pp::Font house_font(DATA_PATH FONT_IMPACTED_PATH, 15);
    SDL2pp::Texture text_sprite_house(this->game_renderer, house_font.RenderText_Blended(house_text, SDL_Color{255, 255, 255, 255}));
    this->game_renderer.Copy(text_sprite_house, SDL2pp::NullOpt, SDL2pp::Rect(SCREEN_WIDTH/5, SCREEN_HEIGHT/4, text_sprite_house.GetWidth(), text_sprite_house.GetHeight()));
    this->game_renderer.Present();
}

void Player::renderButtonInfo(){
    int x, y;
    SDL_GetMouseState(&x, &y);
    int button = -1;
    for(size_t i = 0 ; i < 14 ; i++ ){
        if(this->checkBuild(x,y) != -1){
            button = this->checkBuild(x,y);
            break;    
        }
        if(this->checkUnit(x,y) != -1){
            button = this->checkUnit(x,y);
            break;    
        }
    }
    if(button == -1)
        return;
    SDL2pp::Texture & button_info = this->hud.getButtonInfo(this->checkBtn(x,y),button,this->textures);
    this->game_renderer.Copy(
        button_info,
        SDL2pp::NullOpt,
        SDL2pp::Rect(x-TILE_SIZE*12,y,TILE_SIZE*12,TILE_SIZE*8)
    );    
}

void Player::renderHeldBuilding(){
    int x, y, dim_x, dim_y;
    SDL_GetMouseState(&x, &y);
    if(this->is_holding_building == true){
        SDL2pp::Texture & texture = this->textures.getTexture((building_t)this->building_held,this->faction);
        texture.SetBlendMode(SDL_BLENDMODE_ADD);
        this->game_renderer.Copy(
            texture,
            SDL2pp::NullOpt,
            SDL2pp::Rect(x-TILE_SIZE,y-TILE_SIZE,texture.GetWidth(),texture.GetHeight()));				
        texture.SetBlendMode(SDL_BLENDMODE_BLEND);
    }
}

void Player::render(){
    std::lock_guard<std::mutex> locker(this->lock);
    this->game_renderer.Clear();
    this->updateCamera();
    this->renderMap();
    this->renderCorpses();
    for (auto& e : this->elements)
        e.second->render(this->faction,this->game_renderer,this->camera.pos_x,this->camera.pos_y);
    this->hud.update(this->spice,this->c_spice,this->energy);
    this->shotsHandler.render(this->game_renderer, this->textures, this->camera.pos_x, this->camera.pos_y);
    this->explosionsHandler.render(this->game_renderer, this->textures, this->camera.pos_x, this->camera.pos_y);
    this->renderPrinters();
    this->renderHud();
    this->renderHeldBuilding();
    this->renderButtonInfo();
    this->renderCreators();
    this->renderCreationData();
    if (this->game_status != GAME_NEUTRAL)
        this->renderGameEnding();
    this->game_renderer.Present();
}

void Player::renderPrinters(){
    this->response_printer.renderTimed(this->game_renderer);
    if(this->energy < 0)
        this->energy_printer.render(this->game_renderer);
}

void Player::renderMap(){
    this->game_renderer.SetScale(2,2);
    this->map.render(this->game_renderer,this->textures,this->camera.pos_x,this->camera.pos_y);
}

void Player::renderHud(){
    this->game_renderer.SetScale(1,1);
    this->hud.render(this->game_renderer);
}

void Player::printResponse(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time){
    SDL2pp::Font font(fontpath,size*10);
    SDL2pp::Surface surface = font.RenderText_Solid(toprint,color);
    std::unique_ptr texture = std::make_unique<SDL2pp::Texture>(SDL2pp::Texture(this->game_renderer,surface));
    this->response_printer.timedPrint(std::move(texture),x,y,toprint.size(),size,time);
}

void Player::printEnergy(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time){
    SDL2pp::Font font(fontpath,size*10);
    SDL2pp::Surface surface = font.RenderText_Solid(toprint,color);
    std::unique_ptr texture = std::make_unique<SDL2pp::Texture>(SDL2pp::Texture(this->game_renderer,surface));
    this->energy_printer.print(std::move(texture),x,y,toprint.size(),size);
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

void Player::removeAttacker(int id) {

    this->shotsHandler.removeAttacker(id);
}

void Player::updateAttacker(int id, unit_t type, Position attacker_pos, Position target_pos) {

    this->shotsHandler.updateAttacker(id, type, attacker_pos, target_pos);
}

void Player::addExplosion(int id, selectable_t type, Position pos) {

    this->explosionsHandler.addExplosion(id, type, pos);
}

void Player::addCorpse(selectable_t type, Position position, int time) {

    this->corpses.push_back({type, position, time});
}

void Player::renderCorpses() {

    std::vector<int> corpses_to_remove;
    clock_t current_time = clock();

    for (size_t i = 0; i < this->corpses.size(); i++) {
        if (current_time - this->corpses[i].time > 20000000)
            corpses_to_remove.push_back(i);
        else {

            SDL2pp::Texture & corpse = textures.getCorpse(this->corpses[i].type);
            int pos_x = this->corpses[i].position.x * TILE_SIZE - this->camera.pos_x;
            int pos_y = this->corpses[i].position.y * TILE_SIZE - this->camera.pos_y;
            this->game_renderer.Copy(
                corpse,
                SDL2pp::NullOpt,
                SDL2pp::Rect(pos_x,pos_y,1*TILE_SIZE,1*TILE_SIZE)
            );
        }
    }

    if (corpses_to_remove.size() != 0) {

        for (size_t i = corpses_to_remove.size() - 1; i >= 0; i--)
            this->corpses.erase(this->corpses.begin() + corpses_to_remove[i]);

    }
}

void Player::triggerBaseAttackAlert(clock_t& current_time) {

    if (current_time - this->base_alert_delay > 20000000) {
        this->audio.play(AI_ATACK);
        this->audio.play(UNDER_ATTACK);
        this->base_alert_delay = current_time;
    }
}

void Player::triggerNewUnitAlert(unit_t type) {

    switch (type)
    {
    case INFANTRY:
        this->audio.play(H_ECONF1);
        break;
    case SARDAUKAR:
        this->audio.play(H_ECONF2);
        break;
    case FREMEN:
        this->audio.play(H_ECONF3);
        break;
    case TRIKE:
        this->audio.play(H_ESEL1);
        break;
    case TANK:
        this->audio.play(H_ESEL2);
        break;
    case DEVASTATOR:
        this->audio.play(H_ESEL3);
        break;
    case HARVESTER:
        this->audio.play(FLAMER1);
        break;
    default:
        break;
    }
}

bool Player::positionBelongsToBase(Position position) {

    int map_dim_x = 80;
    int map_dim_y = 45;

    for (size_t i = this->base_position.x; i < this->base_position.x + CONSTRUCTION_YARD_DIM_X; i++) {
        for (size_t j = this->base_position.y; j < this->base_position.y + CONSTRUCTION_YARD_DIM_Y; j++) {
            
            if (i >= map_dim_x || j >= map_dim_y)
                continue;
            if (position.x == i && position.y == j)
                return true;
        }
    }
    return false;
}

void Player::renderGameEnding(){

    if (this->game_status == GAME_NEUTRAL)
        return;

    SDL2pp::Texture & status = textures.getGameStatus(this->game_status);

    this->game_renderer.Copy(
        status,
        SDL2pp::NullOpt,
        SDL2pp::Rect((SCREEN_WIDTH/4)-20, (SCREEN_HEIGHT/4)+20, SCREEN_WIDTH/2, SCREEN_HEIGHT/2)
    );    
}

void Player::updateCamera() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    // SCREEN_WIDTH = 1280
    // SCREEN_HEIGHT = 720
    if (!this->game_has_started)
        this->camera.move(0,1);
    if (this->a_key) this->camera.move(-2,0);
    if (this->d_key) this->camera.move(2,0);
    if (this->w_key) this->camera.move(0,-2);
    if (this->s_key) this->camera.move(0,2);
}

void Player::handleEvent(SDL_Event& event) {

    std::vector<int> new_usr_event;

    int x, y;

    //  Obtenemos la instrucción del mouse
    mouse.getEvent(&event);
    Position current_pos = mouse.currentPosition();

    new_usr_event.clear();

    switch(event.type) {

        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                this->w_key = true;
            if (event.key.keysym.scancode == SDL_SCANCODE_A)
                this->a_key = true;
            if (event.key.keysym.scancode == SDL_SCANCODE_S)
                this->s_key = true;
            if (event.key.keysym.scancode == SDL_SCANCODE_D)
                this->d_key = true;
            break;

        case SDL_KEYUP:
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                this->w_key = false;
            if (event.key.keysym.scancode == SDL_SCANCODE_A)
                this->a_key = false;
            if (event.key.keysym.scancode == SDL_SCANCODE_S)
                this->s_key = false;
            if (event.key.keysym.scancode == SDL_SCANCODE_D)
                this->d_key = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (mouse.leftClick()){
                mouse.click();
                SDL_GetMouseState(&x, &y);
                if (checkHud(x,y)) {
                    mouse.unclick();
                    switch (checkBtn(x, y))
                    {
                    case BUILD_BTN:
                        if(this->construction_time < CONSTRUCTION_TIME+this->time_penalty){
                            this->printResponse(usr_msg[RES_WAIT_TO_BUILD], DATA_PATH FONT_IMPACT_PATH, 200, 300, 10, colors[RED], 1000);   
                            return;
                        } 
                        this->printResponse("Place building on screen", DATA_PATH FONT_IMPACT_PATH, 200, 300, 10, colors[YELLOW], 1000);
                        this->is_holding_building = true;
                        this->building_held = checkBuild(x, y);
                        break;
                    case UNIT_BTN:
                        if (this->new_unit_available) {
                            new_usr_event.push_back(CREATE_UNIT);
                            new_usr_event.push_back(checkUnit(x,y));
                            new_usr_event.push_back(current_pos.x);
                            new_usr_event.push_back(current_pos.y);
                            this->is_holding_building = false;
                            this->building_held = -1;
                            this->usr_events.push(new_usr_event);
                            this->new_unit_available = false;
                        }

                        break;
                    default:
                        this->is_holding_building = false;
                        this->building_held = -1;
                        break;
                    }
                } else if (this->is_holding_building) {
                    new_usr_event.push_back(CREATE_BUILDING);
                    new_usr_event.push_back(this->building_held);
                    new_usr_event.push_back(current_pos.x);
                    new_usr_event.push_back(current_pos.y);
                    this->is_holding_building = false;
                    this->building_held = -1;
                    this->usr_events.push(new_usr_event);
                } else {
                    new_usr_event.push_back(MOUSE_LEFT_CLICK);
                    new_usr_event.push_back(current_pos.x);
                    new_usr_event.push_back(current_pos.y);
                    if (!this->left_click) {
                        this->usr_events.push(new_usr_event);
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
                        new_usr_event.push_back(MOUSE_RIGHT_CLICK);
                        new_usr_event.push_back(current_pos.x);
                        new_usr_event.push_back(current_pos.y);
                        this->usr_events.push(new_usr_event);
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
                    new_usr_event.push_back(MOUSE_SELECTION);
                    new_usr_event.push_back(selection.Xmin);
                    new_usr_event.push_back(selection.Xmax);
                    new_usr_event.push_back(selection.Ymin);
                    new_usr_event.push_back(selection.Ymax);
                    this->usr_events.push(new_usr_event);
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
}

void Player::sendInstruction(command_t command, std::vector<int>& usr_event) {
    //  La pasamos por socket
    this->protocol.send_command(command, this->socket);
    switch (command){
        case CREATE_UNIT:
            this->protocol.send_create_unit_request(usr_event[1], this->socket);
            break;
        case CREATE_BUILDING:
            this->protocol.send_create_building_request(usr_event[1], usr_event[2],usr_event[3],this->socket);
            break;
        case MOUSE_LEFT_CLICK:
            this->protocol.send_mouse_left_click(usr_event[1], usr_event[2], this->socket);
            break;
        case MOUSE_RIGHT_CLICK:
            this->protocol.send_mouse_right_click(usr_event[1], usr_event[2], this->socket);
            break;
        case MOUSE_SELECTION:
            this->protocol.send_mouse_selection(usr_event[1], usr_event[2], usr_event[3], usr_event[4], this->socket);
            break;
        case IDLE:
            break;
        default:
            break;
    }
}

void Player::handleResponse() {
    response_t response;
    int responses_size = 0;
    std::vector<response_t> responses;
    this->protocol.receive_responses_size(responses_size, this->socket);
    for (size_t i = 0; i < responses_size; i++){
        this->protocol.receive_response(response, this->socket);
        responses.push_back(response);
    }
    for (response_t res : responses) {
        if (res != RES_SUCCESS)
            this->printResponse(usr_msg[res], DATA_PATH FONT_IMPACT_PATH, 200, 300, 10, res >= RESPONSE_FAILURE_OFFSET ? colors[GREEN] : colors[RED], 1000);
        if(res == RES_CREATE_BUILDING_SUCCESS)
            this->construction_time = 0;
    }
    //responses.clear();
}

void Player::updateUtils() {
    this->shotsHandler.update();
    this->explosionsHandler.update();
    this->energy_printer.clearPrints();
}

void Player::updateHud(){
    if(this->construction_time < (CONSTRUCTION_TIME + this->time_penalty)){
        this->construction_time+=1;
        this->hud.setBuildButtonColor(this->construction_time/(CONSTRUCTION_TIME+this->time_penalty),faction_colours[this->faction]);
    }
}

void Player::updatePlayerState(){
    int spice,c_spice; 
    std::cout << "T2: Waiting for Update from Server" << std::endl;  
    this->protocol.receive_player_state(spice,c_spice,energy,this->socket);
    std::cout << "Received server data" << std::endl;  
    std::cout << "<========== Player state ========>" << std::endl;
    std::cout << "spice: " << spice << std::endl; 
    std::cout << "spice capacity: " << c_spice  << std::endl; 
    std::cout << "energy: " << energy << std::endl; 

    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    //  Inform energy debt (if any)
    if(energy < 0){
        this->time_penalty = 0;
        this->time_penalty = round(abs(energy))/10;
        this->printEnergy("You are running on an energy debt of: " + std::to_string(abs(energy)), DATA_PATH FONT_IMPACT_PATH, 200, 30, 10, colors[RED], 1000);   
        this->printEnergy("Units & Buildings will take significantly longer to create", DATA_PATH FONT_IMPACT_PATH, 150, 80, 10, colors[RED], 1000);   
    } else { 
        this->time_penalty = 0;
    }
}

void Player::updateBoard(){
    int pos_x,pos_y,spice; //  Values
    int toread,c;    
    this->protocol.receive_sand_cells_size(toread,this->socket);
    std::cout << "Sand cells to read: " << toread <<std::endl;
    for (c = 0 ; c < toread; c++){
        this->protocol.receive_sand_cell(pos_x,pos_y,spice,this->socket);
        this->map.updateCell(this->game_renderer,pos_x,pos_y,spice);
    }
}

void Player::updateSelectables(){
    int id,faction,lp,pos_x,pos_y,speed,direction,energy,spice,c_spice; //  Values
    bool selected,moving,harvesting,attacking,waiting; //  State flags
    int enemy_pos_x,enemy_pos_y; // Enemy positions
    int  toread,c;    
    clock_t current_time = clock();
    //  Set updates to false
    for (size_t i = 0 ; i < this->updates.size(); i++)
        this->updates[i] = false;
    //  Get reads from server
    this->protocol.receive_selectables_to_read(toread, this->socket);
    //  Read each element
    for (size_t i = 0 ; i < toread ; i++){
        selectable_t type;
        this->protocol.receive_selectable_type(type,this->socket);
        switch(type){
            case SEL_TRIKE:
                this->protocol.receive_trike(id,faction,lp,pos_x,pos_y,speed,direction,moving,selected,attacking,enemy_pos_x,enemy_pos_y,waiting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,speed,(direction_t)direction,moving,selected,attacking,waiting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                    if (attacking) {
                        this->updateAttacker(id, TRIKE, Position(pos_x, pos_y), Position(enemy_pos_x, enemy_pos_y));
                        if (this->positionBelongsToBase(Position(enemy_pos_x, enemy_pos_y)))
                            triggerBaseAttackAlert(current_time);
                    } else {
                        this->removeAttacker(id);
                    }
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(TRIKE,id,faction,lp,pos_x,pos_y,1,1,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if ((int) this->faction == faction)
                        this->triggerNewUnitAlert((unit_t) type);
                }
                break;
            case SEL_HARVESTER:
                this->protocol.receive_harvester(id,faction,lp,pos_x,pos_y,speed,direction,moving,selected,spice,harvesting,waiting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,speed,(direction_t)direction,moving,selected,harvesting,waiting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(HARVESTER,id,faction,lp,pos_x,pos_y,1,1,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if ((int) this->faction == faction)
                        this->triggerNewUnitAlert((unit_t) type);
                }
                break;
            case SEL_FREMEN:
                this->protocol.receive_fremen(id,faction,lp,pos_x,pos_y,speed,direction,moving,selected,attacking,enemy_pos_x,enemy_pos_y,waiting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,speed,(direction_t)direction,moving,selected,attacking,waiting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                    if (attacking) {
                        this->updateAttacker(id, FREMEN, Position(pos_x, pos_y), Position(enemy_pos_x, enemy_pos_y));
                        if (this->positionBelongsToBase(Position(enemy_pos_x, enemy_pos_y)))
                            triggerBaseAttackAlert(current_time);
                    } else {
                        this->removeAttacker(id);
                    }
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(FREMEN,id,faction,lp,pos_x,pos_y,1,1,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if ((int) this->faction == faction)
                        this->triggerNewUnitAlert((unit_t) type);
                }
                break;
            case SEL_INFANTRY:
                this->protocol.receive_infantry(id,faction,lp,pos_x,pos_y,speed,direction,moving,selected,attacking,enemy_pos_x,enemy_pos_y,waiting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,speed,(direction_t)direction,moving,selected,attacking,waiting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                    if (attacking) {
                        this->updateAttacker(id, INFANTRY, Position(pos_x, pos_y), Position(enemy_pos_x, enemy_pos_y));
                        if (this->positionBelongsToBase(Position(enemy_pos_x, enemy_pos_y)))
                            triggerBaseAttackAlert(current_time);
                    } else {
                        this->removeAttacker(id);
                    }
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(INFANTRY,id,faction,lp,pos_x,pos_y,1,1,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if ((int) this->faction == faction)
                        this->triggerNewUnitAlert((unit_t) type);
                }
                break;   
            case SEL_SARDAUKAR:
                this->protocol.receive_sardaukar(id,faction,lp,pos_x,pos_y,speed,direction,moving,selected,attacking,enemy_pos_x,enemy_pos_y,waiting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,speed,(direction_t)direction,moving,selected,attacking,waiting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                    if (attacking) {
                        this->updateAttacker(id, SARDAUKAR, Position(pos_x, pos_y), Position(enemy_pos_x, enemy_pos_y));
                        if (this->positionBelongsToBase(Position(enemy_pos_x, enemy_pos_y)))
                            triggerBaseAttackAlert(current_time);
                    } else {
                        this->removeAttacker(id);
                    }
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(SARDAUKAR,id,faction,lp,pos_x,pos_y,1,1,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if ((int) this->faction == faction)
                        this->triggerNewUnitAlert((unit_t) type);
                }
                break;
            case SEL_TANK:
                this->protocol.receive_tank(id,faction,lp,pos_x,pos_y,speed,direction,moving,selected,attacking,enemy_pos_x,enemy_pos_y,waiting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,speed,(direction_t)direction,moving,selected,attacking,waiting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                    if (attacking) {
                        this->updateAttacker(id, TANK, Position(pos_x, pos_y), Position(enemy_pos_x, enemy_pos_y));
                        if (this->positionBelongsToBase(Position(enemy_pos_x, enemy_pos_y)))
                            triggerBaseAttackAlert(current_time);
                    } else {
                        this->removeAttacker(id);
                    }
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(TANK,id,faction,lp,pos_x,pos_y,1,1,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if ((int) this->faction == faction)
                        this->triggerNewUnitAlert((unit_t) type);
                }
                break; 
            case SEL_DEVASTATOR:
                this->protocol.receive_devastator(id,faction,lp,pos_x,pos_y,speed,direction,moving,selected,attacking,enemy_pos_x,enemy_pos_y,waiting,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,speed,(direction_t)direction,moving,selected,attacking,waiting,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                    if (attacking) {
                        this->updateAttacker(id, DEVASTATOR, Position(pos_x, pos_y), Position(enemy_pos_x, enemy_pos_y));
                        if (this->positionBelongsToBase(Position(enemy_pos_x, enemy_pos_y)))
                            triggerBaseAttackAlert(current_time);
                    } else {
                        this->removeAttacker(id);
                    }
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CMovable(DEVASTATOR,id,faction,lp,pos_x,pos_y,1,1,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if ((int) this->faction == faction)
                        this->triggerNewUnitAlert((unit_t) type);
                }
                break; 
            case SEL_CONSTRUCTION_YARD:
                this->protocol.receive_construction_yard(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(CONSTRUCTION_YARD,id,faction,lp,pos_x,pos_y,CONSTRUCTION_YARD_DIM_X,CONSTRUCTION_YARD_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if (faction != (int) this->faction)
                        this->enemies++;
                }  
                break;    
            case SEL_AIR_TRAP:
                this->protocol.receive_air_trap(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(AIR_TRAP,id,faction,lp,pos_x,pos_y,AIR_TRAP_DIM_X,AIR_TRAP_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);  
                    if (faction == (int) this->faction)
                        this->audio.play(CRUSH1);
                }     
                break;
            case SEL_REFINERY:
                this->protocol.receive_refinery(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(REFINERY,id,faction,lp,pos_x,pos_y,REFINERY_DIM_X,REFINERY_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if (faction == (int) this->faction)
                        this->audio.play(CRUSH1);
                } 
                break;
            case SEL_BARRACK:
                this->protocol.receive_barrack(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(BARRACK,id,faction,lp,pos_x,pos_y,BARRACK_DIM_X,BARRACK_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if (faction == (int) this->faction)
                        this->audio.play(CRUSH1);
                }  
                break;   
            case SEL_LIGHT_FACTORY:
                this->protocol.receive_light_factory(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(LIGHT_FACTORY,id,faction,lp,pos_x,pos_y,LIGHT_FACTORY_DIM_X,LIGHT_FACTORY_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if (faction == (int) this->faction)
                        this->audio.play(CRUSH1);
                }  
                break;  
            case SEL_HEAVY_FACTORY:
                this->protocol.receive_heavy_factory(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(HEAVY_FACTORY,id,faction,lp,pos_x,pos_y,HEAVY_FACTORY_DIM_X,HEAVY_FACTORY_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if (faction == (int) this->faction)
                        this->audio.play(CRUSH1);
                }  
                break;      
            case SEL_SILO:
                this->protocol.receive_silo(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(SILO,id,faction,lp,pos_x,pos_y,SILO_DIM_X,SILO_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if (faction == (int) this->faction)
                        this->audio.play(CRUSH1);
                }  
                break;    
            case SEL_PALACE:
                this->protocol.receive_palace(id,faction,lp,pos_x,pos_y,selected,this->socket);
                if (this->contains(id)){
                    this->elements.at(id)->update(this->faction,lp,pos_x,pos_y,0,TOP,false,selected,false,false,this->game_renderer,camera.pos_x,camera.pos_y);
                    this->updates[id] = true;
                } else {
                    this->elements.insert({id,std::unique_ptr<CSelectable>(new CStatic(PALACE,id,faction,lp,pos_x,pos_y,PALACE_DIM_X,PALACE_DIM_Y,this->game_renderer,this->textures,DATA_PATH LP_PATH))});
                    this->updates.push_back(true);
                    if (faction == (int) this->faction)
                        this->audio.play(CRUSH1);
                }  
                break;                      
        }   
    }
    //  Destroy non-updated elements
    for (size_t i = 0 ; i < this->updates.size() ; i++) {
        if(this->updates[i] == false) {
            if(this->contains(i)) {
                int _id = i;
                selectable_t _type = this->elements.at(i)->getType();
                Position _pos = this->elements.at(i)->getPosition();
                this->addExplosion(_id, _type, _pos);
                switch (_type)
                {
                case SEL_INFANTRY: case SEL_FREMEN: case SEL_SARDAUKAR:
                    this->removeAttacker(_id);
                    this->addCorpse(_type, _pos, clock());
                    break;
                case SEL_TRIKE: case SEL_TANK: case SEL_DEVASTATOR:
                    this->removeAttacker(_id);
                    break;
                case SEL_BARRACK:
                    if (_id == this->creators[BARRACK])
                        this->creators[BARRACK] = -1;
                    break;
                case SEL_LIGHT_FACTORY:
                    if (_id == this->creators[LIGHT_FACTORY]) {
                        this->creators[LIGHT_FACTORY] = -1;
                    }
                    break;
                case SEL_HEAVY_FACTORY:
                    if (_id == this->creators[HEAVY_FACTORY])
                        this->creators[HEAVY_FACTORY] = -1;
                    break;
                case SEL_CONSTRUCTION_YARD:
                    if (this->positionBelongsToBase(_pos)) {
                        // Handle game failure
                        this->game_status = GAME_FAILURE;
                        this->audio.stopMusic();
                        this->audio.play(AI_MFAIL);
                        this->audio.play(WAITING_MUSIC);
                    } else {
                        this->destroyed_enemies++;
                        if ((this->enemies == this->destroyed_enemies) && (this->enemies > 0)) {
                            // Handle game success
                            this->game_status = GAME_VICTORY;
                            this->audio.stopMusic();
                            this->audio.play(VICTORY);
                            this->audio.play(VICTORY_MUSIC);
                        }
                    }
                    break;
                default:
                    break;
                }
                this->elements.erase(i);
            }
        }
    }
}