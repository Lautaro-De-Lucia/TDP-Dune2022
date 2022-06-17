#include <iostream>

#include <netinet/in.h>

#include <chrono>
#include <thread>

#include "protocol.h"

Protocol::Protocol() {
}

Protocol::~Protocol() {
}

void Protocol::handle_dispatch(bool was_closed, int sent_size) {

    if (was_closed)
        throw was_closed_exception();
    else if (sent_size < 0)
        throw failure_to_send_exception();
}

void Protocol::handle_receive(bool was_closed, int recv_size) {

    if (was_closed)
        throw was_closed_exception();
    else if (recv_size < 0)
        throw failure_to_receive_exception();
}

// forcing endianness (Big Endian)

void Protocol::send_create_building_request(int type, int pos_x, int pos_y, Socket& client_socket) {

    command_t building_command = CREATE_BUILDING;

    uint8_t _building_command = (uint8_t) building_command;
    uint8_t _type = (uint8_t) type;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;

    uint8_t building_command_buffer = (uint8_t) _building_command - '0';
    uint8_t type_buffer = (uint8_t) _type - '0';
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&building_command_buffer, sizeof(building_command_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_create_unit_request(int type, Socket& client_socket) {

    command_t unit_command = CREATE_UNIT;

    uint8_t _unit_command = (uint8_t) unit_command;
    uint8_t _type = (uint8_t) type;

    uint8_t unit_command_buffer = (uint8_t) _unit_command - '0';
    uint8_t type_buffer = (uint8_t) _type - '0';

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&unit_command_buffer, sizeof(unit_command_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_mouse_left_click(int pos_x, int pos_y, Socket& client_socket) {

    command_t left_click_command = MOUSE_LEFT_CLICK;

    uint8_t _left_click_command = (uint8_t) left_click_command;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;

    uint8_t left_click_command_buffer = (uint8_t) _left_click_command - '0';
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&left_click_command_buffer, sizeof(left_click_command_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_mouse_right_click(int pos_x, int pos_y, Socket& client_socket) {

    command_t right_click_command = MOUSE_RIGHT_CLICK;

    uint8_t _right_click_command = (uint8_t) right_click_command;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;

    uint8_t right_click_command_buffer = (uint8_t) _right_click_command - '0';
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&right_click_command_buffer, sizeof(right_click_command_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_mouse_selection(int pos_x_min, int pos_x_max, int pos_y_min, int pos_y_max, Socket& client_socket) {

    command_t selection_command = MOUSE_SELECTION;

    uint8_t _selection_command = (uint8_t) selection_command;
    uint16_t _pos_x_min = (uint16_t) pos_x_min;
    uint16_t _pos_x_max = (uint16_t) pos_x_max;
    uint16_t _pos_y_min = (uint16_t) pos_y_min;
    uint16_t _pos_y_max = (uint16_t) pos_y_max;

    uint8_t selection_command_buffer = (uint8_t) _selection_command - '0';
    uint16_t pos_x_min_buffer = (uint16_t) htons(_pos_x_min);
    uint16_t pos_x_max_buffer = (uint16_t) htons(_pos_x_max);
    uint16_t pos_y_min_buffer = (uint16_t) htons(_pos_y_min);
    uint16_t pos_y_max_buffer = (uint16_t) htons(_pos_y_max);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&selection_command_buffer, sizeof(selection_command_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_min_buffer, sizeof(pos_x_min_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_max_buffer, sizeof(pos_x_max_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_min_buffer, sizeof(pos_y_min_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_max_buffer, sizeof(pos_y_max_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_idle(Socket& client_socket) {

    command_t idle_command = IDLE;

    uint8_t _idle_command = (uint8_t) idle_command;

    uint8_t idle_command_buffer = (uint8_t) _idle_command - '0';

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&idle_command_buffer, sizeof(idle_command_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_command(command_t& command, Socket& client_socket) {
    
    uint8_t new_command_buffer;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&new_command_buffer, sizeof(new_command_buffer), &was_closed);
    handle_receive(was_closed, recv_size);

    command = (command_t) new_command_buffer;

    return;
}

void Protocol::receive_create_building_request(int& type, int& pos_x, int& pos_y, Socket& client_socket) {

    uint8_t type_buffer;
    uint16_t pos_x_buffer;
    uint16_t pos_y_buffer;

    uint8_t _type;
    uint16_t _pos_x;
    uint16_t _pos_y;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _type = (uint8_t) type_buffer;

    recv_size = client_socket.recvall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_x = (uint16_t) ntohs(pos_x_buffer);

    recv_size = client_socket.recvall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_y = (uint16_t) ntohs(pos_y_buffer);

    type = (int) _type;
    pos_x = (int) _pos_x;
    pos_y = (int) _pos_y;

    return;
}

void Protocol::receive_create_unit_request(int& type, Socket& client_socket) {

    uint8_t type_buffer;

    uint8_t _type;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _type = (uint8_t) type_buffer;

    type = (int) _type;

    return;
}

void Protocol::receive_mouse_left_click(int& pos_x, int& pos_y, Socket& client_socket) {

    uint16_t pos_x_buffer;
    uint16_t pos_y_buffer;

    uint16_t _pos_x;
    uint16_t _pos_y;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_x = (uint16_t) ntohs(pos_x_buffer);

    recv_size = client_socket.recvall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_y = (uint16_t) ntohs(pos_y_buffer);

    pos_x = (int) _pos_x;
    pos_y = (int) _pos_y;

    return;
}

void Protocol::receive_mouse_right_click(int& pos_x, int& pos_y, Socket& client_socket) {

    uint16_t pos_x_buffer;
    uint16_t pos_y_buffer;

    uint16_t _pos_x;
    uint16_t _pos_y;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_x = (uint16_t) ntohs(pos_x_buffer);

    recv_size = client_socket.recvall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_y = (uint16_t) ntohs(pos_y_buffer);

    pos_x = (int) _pos_x;
    pos_y = (int) _pos_y;

    return;
}

void Protocol::receive_mouse_selection(int& pos_x_min, int& pos_x_max, int& pos_y_min, int& pos_y_max, Socket& client_socket) {

    uint16_t pos_x_min_buffer;
    uint16_t pos_x_max_buffer;
    uint16_t pos_y_min_buffer;
    uint16_t pos_y_max_buffer;

    uint16_t _pos_x_min;
    uint16_t _pos_x_max;
    uint16_t _pos_y_min;
    uint16_t _pos_y_max;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&pos_x_min_buffer, sizeof(pos_x_min_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_x_min = (uint16_t) ntohs(pos_x_min_buffer);

    recv_size = client_socket.recvall(&pos_x_max_buffer, sizeof(pos_x_max_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_x_max = (uint16_t) ntohs(pos_x_max_buffer);

    recv_size = client_socket.recvall(&pos_y_min_buffer, sizeof(pos_y_min_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_y_min = (uint16_t) ntohs(pos_y_min_buffer);

    recv_size = client_socket.recvall(&pos_y_max_buffer, sizeof(pos_y_max_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_y_max = (uint16_t) ntohs(pos_y_max_buffer);

    pos_x_min = (int) _pos_x_min;
    pos_x_max = (int) _pos_x_max;
    pos_y_min = (int) _pos_y_min;
    pos_y_max = (int) _pos_y_max;

    return;
}

void Protocol::send_command_response(response_t response, Socket& client_socket) {

    uint8_t _response = (uint8_t) response;

    uint8_t response_buffer = (uint8_t) _response - '0';

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&response_buffer, sizeof(response_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_command_response(response_t& response, Socket& client_socket) {

    uint8_t new_response_buffer;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&new_response_buffer, sizeof(new_response_buffer), &was_closed);
    handle_receive(was_closed, recv_size);

    response = (response_t) new_response_buffer;

    return;
}

void Protocol::send_selectable_type(selectable_t type, Socket& client_socket) {

    uint8_t _type = (uint8_t) type;

    uint8_t type_buffer = (uint8_t) _type - '0';

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_selectable_type(selectable_t& type, Socket& client_socket) {

    uint8_t type_buffer;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_receive(was_closed, recv_size);

    type = (selectable_t) type_buffer;

    return;
}

void Protocol::send_element(Trike& trike, int ID, Socket& client_socket) {

    this->send_selectable_type(SEL_TRIKE, client_socket);

    int id = ID;
    int lp = trike.getLP();
    int pos_x = trike.getPosition().x;
    int pos_y = trike.getPosition().y;
    bool selected = trike.isSelected();
    bool attacking = trike.isAttacking();

    uint16_t _id = (uint16_t) id;
    uint16_t _lp = (uint16_t) lp;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;
    uint8_t _selected = (uint8_t) selected;
    uint8_t _attacking = (uint8_t) attacking;

    uint16_t id_buffer = (uint16_t) htons(_id);
    uint16_t lp_buffer = (uint16_t) htons(_lp);
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);
    uint8_t selected_buffer = (uint8_t) _selected - '0';
    uint8_t attacking_buffer = (uint8_t) _attacking - '0';

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&lp_buffer, sizeof(lp_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&selected_buffer, sizeof(selected_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_element(Harvester& harvester, int ID, Socket& client_socket) {

    this->send_selectable_type(SEL_HARVESTER, client_socket);

    int id = ID;
    int lp = harvester.getLP();
    int pos_x = harvester.getPosition().x;
    int pos_y = harvester.getPosition().y;
    bool selected = harvester.isSelected();
    int spice = harvester.getSpice();
    bool harvesting = harvester.isHarvesting();

    uint16_t _id = (uint16_t) id;
    uint16_t _lp = (uint16_t) lp;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;
    uint8_t _selected = (uint8_t) selected;
    uint16_t _spice = (uint16_t) spice;
    uint8_t _harvesting = (uint8_t) harvesting;

    uint16_t id_buffer = (uint16_t) htons(_id);
    uint16_t lp_buffer = (uint16_t) htons(_lp);
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);
    uint8_t selected_buffer = (uint8_t) _selected - '0';
    uint16_t spice_buffer = (uint16_t) htons(_spice);
    uint8_t harvesting_buffer = (uint8_t) _harvesting - '0';

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&lp_buffer, sizeof(lp_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&selected_buffer, sizeof(selected_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&harvesting_buffer, sizeof(harvesting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;

}

void Protocol::send_element(AirTrap& air_trap, int ID, Socket& client_socket) {

    this->send_selectable_type(SEL_AIR_TRAP, client_socket);

    int id = ID;
    int lp = air_trap.getLP();
    int pos_x = air_trap.getPosition().x;
    int pos_y = air_trap.getPosition().y;
    bool selected = air_trap.isSelected();
    int c_energy = air_trap.getCEnergy();
    int spice = air_trap.getSpice();
    int energy = air_trap.getEnergy();

    uint16_t _id = (uint16_t) id;
    uint16_t _lp = (uint16_t) lp;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;
    uint8_t _selected = (uint8_t) selected;
    uint16_t _c_energy = (uint16_t) c_energy;
    uint16_t _spice = (uint16_t) spice;
    uint16_t _energy = (uint16_t) energy;

    uint16_t id_buffer = (uint16_t) htons(_id);
    uint16_t lp_buffer = (uint16_t) htons(_lp);
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);
    uint8_t selected_buffer = (uint8_t) _selected - '0';
    uint16_t c_energy_buffer = (uint16_t) htons(_c_energy);
    uint16_t spice_buffer = (uint16_t) htons(_spice);
    uint16_t energy_buffer = (uint16_t) htons(_energy);


    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&lp_buffer, sizeof(lp_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&selected_buffer, sizeof(selected_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&c_energy_buffer, sizeof(c_energy_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_element(Barrack& barrack, int ID, Socket& client_socket) {

    this->send_selectable_type(SEL_BARRACK, client_socket);

    int id = ID;
    int lp = barrack.getLP();
    int pos_x = barrack.getPosition().x;
    int pos_y = barrack.getPosition().y;
    bool selected = barrack.isSelected();
    int spice = barrack.getSpice();
    int energy = barrack.getEnergy();

    uint16_t _id = (uint16_t) id;
    uint16_t _lp = (uint16_t) lp;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;
    uint8_t _selected = (uint8_t) selected;
    uint16_t _spice = (uint16_t) spice;
    uint16_t _energy = (uint16_t) energy;

    uint16_t id_buffer = (uint16_t) htons(_id);
    uint16_t lp_buffer = (uint16_t) htons(_lp);
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);
    uint8_t selected_buffer = (uint8_t) _selected - '0';
    uint16_t spice_buffer = (uint16_t) htons(_spice);
    uint16_t energy_buffer = (uint16_t) htons(_energy);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&lp_buffer, sizeof(lp_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&selected_buffer, sizeof(selected_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_element(Refinery& refinery, int ID, Socket& client_socket) {

    this->send_selectable_type(SEL_REFINERY, client_socket);

    int id = ID;
    int lp = refinery.getLP();
    int pos_x = refinery.getPosition().x;
    int pos_y = refinery.getPosition().y;
    bool selected = refinery.isSelected();
    int c_spice = refinery.getCSpice();
    int spice = refinery.getSpice();
    int energy = refinery.getEnergy();

    uint16_t _id = (uint16_t) id;
    uint16_t _lp = (uint16_t) lp;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;
    uint8_t _selected = (uint8_t) selected;
    uint16_t _c_spice = (uint16_t) c_spice;
    uint16_t _spice = (uint16_t) spice;
    uint16_t _energy = (uint16_t) energy;

    uint16_t id_buffer = (uint16_t) htons(_id);
    uint16_t lp_buffer = (uint16_t) htons(_lp);
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);
    uint8_t selected_buffer = (uint8_t) _selected - '0';
    uint16_t c_spice_buffer = (uint16_t) htons(_c_spice);
    uint16_t spice_buffer = (uint16_t) htons(_spice);
    uint16_t energy_buffer = (uint16_t) htons(_energy);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&lp_buffer, sizeof(lp_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&selected_buffer, sizeof(selected_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&c_spice_buffer, sizeof(c_spice_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_trike(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket) {

    this->receive_element(id, lp, pos_x, pos_y, selected, client_socket);

    uint8_t attacking_buffer;
    uint8_t _attacking;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _attacking = (uint8_t) attacking_buffer;
    attacking = (bool) _attacking;

    return;
}

void Protocol::receive_harvester(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, int& spice, bool& harvesting, Socket& client_socket) {

    this->receive_element(id, lp, pos_x, pos_y, selected, client_socket);

    uint8_t harvesting_buffer;
    uint8_t _harvesting;
    uint16_t spice_buffer;
    uint16_t _spice;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&harvesting_buffer, sizeof(harvesting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _harvesting = (uint8_t) harvesting_buffer;
    recv_size = client_socket.recvall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _spice = (uint16_t) ntohs(spice_buffer);

    harvesting = (bool) _harvesting;
    spice = (int) _spice;

    return;
}

void Protocol::receive_air_trap(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, int& c_energy, int& spice, int& energy, Socket& client_socket) {

    this->receive_element(id, lp, pos_x, pos_y, selected, client_socket);

    uint16_t c_energy_buffer;
    uint16_t _c_energy;
    uint16_t spice_buffer;
    uint16_t _spice;
    uint16_t energy_buffer;
    uint16_t _energy;
    
    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&c_energy_buffer, sizeof(c_energy_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _c_energy = ntohs(c_energy_buffer);
    recv_size = client_socket.recvall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _spice = ntohs(spice_buffer);
    recv_size = client_socket.recvall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _energy = ntohs(c_energy_buffer);

    c_energy = (int) _c_energy;
    spice = (int) _spice;
    energy = (int) _energy;

    return;
}

void Protocol::receive_barrack(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, int& spice, int& energy, Socket& client_socket) {

    this->receive_element(id, lp, pos_x, pos_y, selected, client_socket);

    uint16_t spice_buffer;
    uint16_t _spice;
    uint16_t energy_buffer;
    uint16_t _energy;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _spice = ntohs(spice_buffer);
    recv_size = client_socket.recvall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _energy = ntohs(energy_buffer);

    spice = (int) _spice;
    energy = (int) _energy;

    return;
}

void Protocol::receive_refinery(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, int& c_spice, int& spice, int& energy, Socket& client_socket) {

    this->receive_element(id, lp, pos_x, pos_y, selected, client_socket);

    int recv_size = -1;
    bool was_closed = false;

    uint16_t c_spice_buffer;
    uint16_t _c_spice;
    uint16_t spice_buffer;
    uint16_t _spice;
    uint16_t energy_buffer;
    uint16_t _energy;

    recv_size = client_socket.recvall(&c_spice_buffer, sizeof(c_spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _c_spice = ntohs(c_spice_buffer);
    recv_size = client_socket.recvall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _spice = ntohs(spice_buffer);
    recv_size = client_socket.recvall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _energy = ntohs(energy_buffer);

    c_spice = (int) _c_spice;
    spice = (int) _spice;
    energy = (int) _energy;
    
    return;
}

void Protocol::receive_element(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    uint16_t id_buffer;
    uint16_t lp_buffer;
    uint16_t pos_x_buffer;
    uint16_t pos_y_buffer;
    uint8_t selected_buffer;

    uint16_t _id;
    uint16_t _lp;
    uint16_t _pos_x;
    uint16_t _pos_y;
    uint8_t _selected;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _id = (uint16_t) ntohs(id_buffer);

    recv_size = client_socket.recvall(&lp_buffer, sizeof(lp_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _lp = (uint16_t) ntohs(lp_buffer);

    recv_size = client_socket.recvall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_x = (uint16_t) ntohs(pos_x_buffer);

    recv_size = client_socket.recvall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_y = (uint16_t) ntohs(pos_y_buffer);

    recv_size = client_socket.recvall(&selected_buffer, sizeof(selected_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _selected = (uint8_t) selected_buffer;

    id = (int) _id;
    lp = (int) _lp;
    pos_x = (int) _pos_x;
    pos_y = (int) _pos_y;
    selected = (bool) _selected;

    return;
}



/*

//  Client::updateBoard(int toread){
//      int pos_x;
//      int pos_y;
//      int type;
//      for (size_t i = 0; i < toread; i++){    
//          this->protocol.receiveCell(pos_x,pos_y,type);
//          this->map_cells[pos_x][pos_y].setType(type);
//      }
//  }




//  Client::updateElements(int toread){
    //  C: [0,1,2,3,4,5]
    //  U: [0t,1t,2t,3t,4t,5t]
    //  while(i < this->updates.size()){this->updates[i] == false};  
    //  U: [0f,1f,2f,3f,4f,5f]
    //  while(i < toread){
    //  S: [0,1,2,4,5,6,7,8]
    //  this->protocol.receive_selectable_type(type,this->socket)  
    //  switch(type)
    //      case TRIKE: 
    //          this->protocol.receive_trike(id,lp,pos_x,pos_y,selected,attacking);
    //          if(this->contains(id))
    //              this->elements.at(id).update(lp,pos_x,pos_y,selected,attacking);
    //              this->updates[id] == true;
    //          else   
    //              this->elements.add({id,new Trike(TRIKE,lp,pos_x,pos_y,selected,attacking)})
    //              this->updates.push_back(true);
    //           // (ATTR: std::vector<bool> updates)
    //      ...
    //      case REFINERY:
    //          this->protocol.receive_refinery(id,lp,selected);
    //          this->elements.at(id).update(lp,selected);
    //      
    //      }
    //      i++
    //      [0t,1t,2t,3f,4t,5t,6t,7t,8t]
    //  }
    //      //  Destroy elements
    //      for (size_t i = 0 ; i < this->updates.size() ; i++)
    //          if(this->updates[i] == false)
    //              if(this->elements.contains(i))
    //                  this->elements.erase(i);
    //      
    //  void receive_trike(int & id, int & lp, int & pos_x, int & pos_y, bool selected, bool attacking){
    //      this->receiveUnit(id,lp,pos_x,pos_y,selected);      
    //      // lectura del attacking
    //      attacking <- lectura;


//  Client::destroyElements(){
//      std::vector<int> todestroy;
//      for (size_t i = 0 ; i < this->updates.size() ; i++){
//          if(this->contains(i) && this->updates[i] == false)
//              todestroy.push_back(i);
//      }
//      for(int id_to_destroy : this->todestroy)
//        this->elements.erase(id_to_destroy);
//  }

*/

void Protocol::send_selectables_size(int size, Socket& client_socket) {

    uint16_t _size = (uint16_t) size;
    uint16_t size_buffer = (uint16_t) htons(_size);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&size_buffer, sizeof(size_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_selectables_size(int& size, Socket& client_socket) {

    uint16_t size_buffer;
    uint16_t _size;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&size_buffer, sizeof(size_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _size = (uint16_t) ntohs(size_buffer);

    size = (int) _size;

    return;
}


void Protocol::send_sand_cells_size(int size, Socket& client_socket) {

    uint16_t _size = (uint16_t) size;
    uint16_t size_buffer = (uint16_t) htons(_size);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&size_buffer, sizeof(size_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_sand_cells_size(int& size, Socket& client_socket) {

    uint16_t size_buffer;
    uint16_t _size;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&size_buffer, sizeof(size_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _size = (uint16_t) ntohs(size_buffer);

    size = (int) _size;

    return;
}

void Protocol::send_sand_cell(int pos_x, int pos_y, int spice, Socket& client_socket) {

    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;
    uint16_t _spice = (uint16_t) spice;

    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);
    uint16_t spice_buffer = (uint16_t) htons(_spice);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_sand_cell(int& pos_x, int& pos_y, int& spice, Socket& client_socket) {

    uint16_t pos_x_buffer;
    uint16_t pos_y_buffer;
    uint16_t spice_buffer;

    uint16_t _pos_x;
    uint16_t _pos_y;
    uint16_t _spice;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_x = (uint16_t) ntohs(pos_x_buffer);

    recv_size = client_socket.recvall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _pos_y = (uint16_t) ntohs(pos_y_buffer);

    recv_size = client_socket.recvall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _spice = (uint16_t) ntohs(spice_buffer);

    pos_x = (int) _pos_x;
    pos_y = (int) _pos_y;
    spice = (int) _spice;

    return;
}
