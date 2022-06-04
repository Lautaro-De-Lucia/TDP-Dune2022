#include "client_player.h"

void CPlayer::addElement(unit_t type,State & desc){

}

void CPlayer::addElement(building_t type,State & desc){

}

void CPlayer::update(std::vector<State> & server_data){
    for (State data : server_data)
        this->elements.at(data.ID).update(data);
}