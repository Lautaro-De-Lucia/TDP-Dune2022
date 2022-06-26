#   Servidor

##  Atributos

    TSQ<std::unique_ptr<instruction_t>>;
    std::vector<ClientHandler> players;


##  ¿Cómo enviamos los comandos?

####    Estructura Genérica 

    struct instruction_t {
        int ID;
        player_t faction;
        command_t command;
    }

####    Con herederas para cada tipo de respuesta

    struct BuildingCreate : instruction_t {
        building_t building_type;
        int pos_x;
        int pos_y;
    }

    void Server::acceptPlayers() {
        this->running = true;
        size_t i = 1;
        while (running == true){
            Socket client_socket = (this->socket.accept());
            this->players.push_back(std::unique_ptr<ClientHandler>(new ClientHandler(this->players.size(),INIT_ENERGY,INIT_SPICE,std::move(client_socket),this->TSQ)));
            checkForFinishedClients();
        }
        closeAllClients();
    }

    void Server::run(){
        this->checkForLosingPlayers();
        while(!TSQ.isFull()){}
        for(size_t i = 0 ; i <= TSQ.size(); i++) {
        std::unique_ptr<instruction_t> new_instruction = this->TSQ.pop();
        this->handleInstruction(*new_instruction);
        }
        this->update();
        for(ClientHandler player : this->players)
            this->reportState(this->game);
    }

        Server::handleInstruction(BuildingCreate instruction){
            int current_spice = this->players[instruction.player_ID].getSpice();
            int current_energy = this->players[instruction.player_ID].getEnergy();
            this->game->createBuilding(instruction.faction,instruction.type,instruction.pos_x,instruction.pos_y,current_spice,current_energy);
            this->players[instruction.player_ID].setSpice(current_spice);
            this->players[instruction.player_ID].setEnergy(current_energy); 
        }


#   Nuevo ClientHandler

Ahora recibe una referencia a la Thread Safe Queue

    ClientHandler::ClientHandler(int player_ID,int init_energy, int init_spice ,Socket && client_socket,TSQ & instruction_queue)
    :
    id(player_ID)
    spice(init_spice),
    energy(init_energy),
    finished(false),
    instructions{instruction_queue},
    thread(&ClientHandler::run,this,std::move(client_socket))
    {
        this->faction = (player_t) -1;

        for (unit_t UNIT : units)
            this->units_to_create[UNIT] = 0,
            this->unit_time[UNIT] = 0,
            this->unit_creation_time[UNIT] = 2;
    }

    void  ClientHandler::run(Socket && client_socket){
        this->initPlayer(client_socket);
        auto base_time_instruction = clock();
        
        command_t command;
        this->protocol.receive_command(command, client_socket);
        response_t res;
        if(command == CLOSE){
            this->finished = true;
            break;
        }
        
        int type, pos_x, pos_y, pos_x_min,pos_x_max,pos_y_min,pos_y_max;

        while(true){
            switch (command){
                case CREATE_UNIT:
                    this->protocol.receive_create_unit_request(type, client_socket);
                    res = queueUnit((unit_t)type);
                    this->instructions();
                    break;
                case CREATE_BUILDING:
                    this->protocol.receive_create_building_request(type, pos_x, pos_y, client_socket);
                    this->instructions.push(BuildingCreate(this->faction,CREATE_BUILDING,type,pos_x,pos_y));
                    break;  
                case MOUSE_LEFT_CLICK:
                    this->protocol.receive_mouse_left_click(pos_x, pos_y, client_socket);
                    handleLeftClick(pos_x, pos_y);
                    res = RES_SUCCESS;
                    break;
                case MOUSE_RIGHT_CLICK:
                    this->protocol.receive_mouse_right_click(pos_x, pos_y, client_socket);
                    handleRightClick(pos_x, pos_y);
                    res = RES_SUCCESS;
                    break;
                case MOUSE_SELECTION:
                    this->protocol.receive_mouse_selection(pos_x_min, pos_x_max, pos_y_min, pos_y_max, client_socket);
                    handleSelection(pos_x_min, pos_x_max, pos_y_min, pos_y_max);
                    res = RES_SUCCESS;
                    break;
                case IDLE:
                    res = RES_SUCCESS;
                    break;
                default:
                    res = RES_SUCCESS;
                    break;
        }
    }

    ClientHandler::initPlayer(Socket & client_socket){
    
    int _faction;
    this->protocol.receive_faction_request(_faction, client_socket);
    this->faction = (player_t) _faction;

    if(this->faction == ATREIDES)
        this->game->createBuilding(this->faction,CONSTRUCTION_YARD,ATREIDES_INIT_POS_X,ATREIDES_INIT_POS_Y,this->spice, this->energy); 
    if(this->faction == HARKONNEN)
        this->game->createBuilding(this->faction,CONSTRUCTION_YARD,HARKONNEN_INIT_POS_X,HARKONNEN_INIT_POS_Y,this->spice, this->energy); 
    if(this->faction == ORDOS)
        this->game->createBuilding(this->faction,CONSTRUCTION_YARD,ORDOS_INIT_POS_X,ORDOS_INIT_POS_Y,this->spice, this->energy); 
    }