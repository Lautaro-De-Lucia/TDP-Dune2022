#include "client_player.h"

CPlayer::CPlayer(SDL2pp::Window & window,SDL2pp::Renderer & renderer,size_t spice, size_t cspice, int energy, size_t c_energy, std::vector<std::vector<cell_t>> & map_data)
:   
game_window(window),
game_renderer(renderer),
map(renderer,map_data)
{
    this->spice = spice;
    this->c_spice = c_spice;
    this->energy = energy;
    this->c_energy = c_energy;
    this->efficiency = efficiency;
}

void CPlayer::addElement(unit_t type,State & desc,SDL2pp::Renderer & renderer){
    //  Ahora estamos hablando de SPRITES
    //  Va a ser una imágen con la unidad en muchas posiciones
    //  Los cortes quedan a cargo de la unidad
    switch(type){
        case HARVESTER: 
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CMovable("Harvester",desc,renderer,DATA_PATH HARVESTER_PATH))});
            break;
        case TRIKE:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CMovable("Trike",desc,renderer,DATA_PATH TRIKE_PATH))});
            break;    
    }
}

void CPlayer::addElement(building_t type,State & desc,SDL2pp::Renderer & renderer){
    switch(type){
        case AIR_TRAP: 
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic("Wind Trap",desc,renderer,DATA_PATH WIND_TRAP_PATH))});
            break;
        case BARRACK:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic("Barrack",desc,renderer,DATA_PATH BARRACK_PATH))});
            break; 
        case REFINERY:
            this->elements.insert({desc.ID,std::unique_ptr<CSelectable>(new CStatic("Refinery",desc,renderer,DATA_PATH REFINERY_PATH))});
            break;       
    }
}

void CPlayer::update(std::vector<State> & server_data,SDL2pp::Renderer & renderer){
    for (State data : server_data){
        std::cout << "Actualizando valores para la unidad: " << data.ID << std::endl;
        (*(this->elements.at(data.ID))).update(data,renderer);  
    }  
}

void CPlayer::renderMap(SDL2pp::Renderer & renderer){
    this->map.render(renderer);
}