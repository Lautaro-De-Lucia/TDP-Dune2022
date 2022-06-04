#include "client_selectable.h"

void CSelectable::update(State & new_state){
    this->LP = new_state.LP;
    this->position = new_state.position;
    this->selected = new_state.selected;
}