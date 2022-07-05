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
}

void Player::play(){

    AudioPlayer audio;

    this->renderWaitingText();

    //  Cargar Texturas

    this->protocol.receive_game_has_started(this->socket);
    this->protocol.send_faction_request(this->faction, this->socket);

    audio.play(GAME_START);

    switch (faction)
    {
    case ATREIDES: audio.play(ATREIDES_MUSIC); break;
    case HARKONNEN: audio.play(HARKONNEN_MUSIC); break;
    case ORDOS: audio.play(ORDOS_MUSIC); break;    
    default: break;
    }

    bool game_has_started = false;
    auto base_time_instruction = clock();
    auto base_time_update = clock();
    std::vector<int> new_mouse_event;

    while (true) {
        //  loopeamos infinitamente
        //  Actualizamos la cámara

        SDL_Event event;

        bool quit = false;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                std::cout << "Leaving..." << std::endl;
                this->protocol.send_command(CLOSE, this->socket);
                quit = true;
                sleep(1);
                break; // salir del otro loop también
            }
            
            int x, y;

            SDL_GetMouseState(&x, &y);


            // SCREEN_WIDTH = 1280
            // SCREEN_HEIGHT = 720

            if (!game_has_started)
                this->camera.move(0,1);

            if (this->a_key) this->camera.move(-2,0);
            if (this->d_key) this->camera.move(2,0);
            if (this->w_key) this->camera.move(0,-2);
            if (this->s_key) this->camera.move(0,2);

            //  Obtenemos la instrucción del mouse
            mouse.getEvent(&event);
            Position current_pos = mouse.currentPosition();

            new_mouse_event.clear();

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
                                    continue;
                                } 
                                this->printResponse("Place building on screen", DATA_PATH FONT_IMPACT_PATH, 200, 300, 10, colors[YELLOW], 1000);
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
                        } else {
                            new_mouse_event.push_back(MOUSE_LEFT_CLICK);
                            new_mouse_event.push_back(current_pos.x);
                            new_mouse_event.push_back(current_pos.y);
                            if (!this->left_click) {
                                this->mouse_events.push(new_mouse_event);
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

        if (quit)
            break;

        this->render();

        auto current_time = clock();

	    auto frame_time_instruction = current_time - base_time_instruction;

        this->shotsHandler.update();
        this->explosionsHandler.update();

        sleepcp(20);

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

        if (this->game_status != GAME_NEUTRAL) {
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

        responses.clear();

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

    this->energy_printer.clearPrints();

    if(this->construction_time < (CONSTRUCTION_TIME + this->time_penalty)){
        //std::cout <<"construction time: "<< construction_time << std::endl;
        this->construction_time+=1;
        this->hud.setBuildButtonColor(this->construction_time/(CONSTRUCTION_TIME+this->time_penalty),faction_colours[this->faction]);
    }
    //std::cout << "Construction time: "<<construction_time << std::endl;
    //  Setup Variables
    int id,faction,lp,pos_x,pos_y,speed,direction,energy,spice,c_spice; //  Values
    bool selected,moving,harvesting,attacking,waiting; //  State flags
    int enemy_pos_x,enemy_pos_y; // Enemy positions
    int  toread,c;                      //  Helper variables
    //  Update values for player
    this->protocol.receive_player_state(spice,c_spice,energy,this->socket);
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;

    if(energy < 0){
        this->time_penalty = 0;
        this->time_penalty = round(abs(energy))/10;
        this->printEnergy("You are running on an energy debt of: " + std::to_string(abs(energy)), DATA_PATH FONT_IMPACT_PATH, 200, 30, 10, colors[RED], 1000);   
        this->printEnergy("Units & Buildings will take significantly longer to create", DATA_PATH FONT_IMPACT_PATH, 150, 80, 10, colors[RED], 1000);   
    } else { 
        this->time_penalty = 0;
    }
    
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

    clock_t current_time = clock();

    //  Receive elements
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
    //  Receive creator state
    this->receiveCreationData();
    //  Receive creators
    this->receiveCreators();
}

void Player::receiveCreationData() {   
    this->creation_data.clear(); 
    int size,creator_ID,unit,current_time,total_time;
    this->protocol.receive_creation_data_size(size, this->socket);
    for (size_t i = 0; i < size ; i++)
        this->protocol.receive_creation_data(creator_ID,unit,current_time,total_time,this->socket),
        this->creation_data.push_back(creation_t(creator_ID,(unit_t)unit,current_time,total_time));
}

void Player::receiveCreators() {

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

/*º
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
    this->game_renderer.Clear();
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

    for (size_t i = 0; i < corpses_to_remove.size(); i++)
            this->corpses.erase(this->corpses.begin() + corpses_to_remove[i]);
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
