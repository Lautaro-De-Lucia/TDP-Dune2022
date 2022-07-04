#include <iostream>

#include <netinet/in.h>

#include <chrono>
#include <thread>

#include "common_protocol.h"

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

void Protocol::send_faction_request(int faction, Socket& client_socket) {

    uint8_t _faction = (uint8_t) faction;

    uint8_t faction_buffer = (uint8_t) _faction;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&faction_buffer, sizeof(faction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;

}

void Protocol::receive_faction_request(int& faction, Socket& client_socket) {

    uint8_t faction_buffer;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&faction_buffer, sizeof(faction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);

    faction = (command_t) faction_buffer;

    return;
}

void Protocol::send_faction_request_response(bool success, Socket& client_socket) {

    uint8_t _success = (uint8_t) success;

    uint8_t success_buffer = (uint8_t) _success;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&success_buffer, sizeof(success_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_faction_request_response(bool& success, Socket& client_socket) {

    uint8_t success_buffer;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&success_buffer, sizeof(success_buffer), &was_closed);
    handle_receive(was_closed, recv_size);

    success = (bool) success_buffer;

    return;

}

void Protocol::send_game_has_started(Socket& client_socket) {

    uint8_t _game_has_started = (uint8_t) true;

    uint8_t game_has_started_buffer = (uint8_t) _game_has_started;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&game_has_started_buffer, sizeof(game_has_started_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_game_has_started(Socket& client_socket) {

    uint8_t game_has_started_buffer;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&game_has_started_buffer, sizeof(game_has_started_buffer), &was_closed);
    handle_receive(was_closed, recv_size);

    return;
}

void Protocol::send_command(command_t command, Socket& client_socket) {

    uint8_t _command = (uint8_t) command;

    uint8_t command_buffer = (uint8_t) _command;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&command_buffer, sizeof(command_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_create_building_request(int type, int pos_x, int pos_y, Socket& client_socket) {

    uint8_t _type = (uint8_t) type;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;

    uint8_t type_buffer = (uint8_t) _type;
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);

    int sent_size = -1;
    bool was_closed = false;
    
    sent_size = client_socket.sendall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_create_unit_request(int type, Socket& client_socket) {

    uint8_t _type = (uint8_t) type;

    uint8_t type_buffer = (uint8_t) _type;

    int sent_size = -1;
    bool was_closed = false;
    
    sent_size = client_socket.sendall(&type_buffer, sizeof(type_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_mouse_left_click(int pos_x, int pos_y, Socket& client_socket) {

    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;

    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_mouse_right_click(int pos_x, int pos_y, Socket& client_socket) {

    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;

    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_mouse_selection(int pos_x_min, int pos_x_max, int pos_y_min, int pos_y_max, Socket& client_socket) {

    uint16_t _pos_x_min = (uint16_t) pos_x_min;
    uint16_t _pos_x_max = (uint16_t) pos_x_max;
    uint16_t _pos_y_min = (uint16_t) pos_y_min;
    uint16_t _pos_y_max = (uint16_t) pos_y_max;

    uint16_t pos_x_min_buffer = (uint16_t) htons(_pos_x_min);
    uint16_t pos_x_max_buffer = (uint16_t) htons(_pos_x_max);
    uint16_t pos_y_min_buffer = (uint16_t) htons(_pos_y_min);
    uint16_t pos_y_max_buffer = (uint16_t) htons(_pos_y_max);

    int sent_size = -1;
    bool was_closed = false;

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

void Protocol::send_response(response_t response, Socket& client_socket) {

    uint8_t response_buffer = (uint8_t) response;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&response_buffer, sizeof(response_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_response(response_t& response, Socket& client_socket) {

    uint8_t new_response_buffer;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&new_response_buffer, sizeof(new_response_buffer), &was_closed);
    handle_receive(was_closed, recv_size);

    response = (response_t) new_response_buffer;

    return;
}

void Protocol::send_creation_data(int creator_ID, int unit, int current_time, int total_time, Socket& client_socket){

    uint16_t _ID = (int16_t) creator_ID;
    uint16_t ID_buffer = (int16_t) htons(_ID);

    uint8_t _unit = (uint8_t) unit;
    uint8_t unit_buffer = (uint8_t) _unit;

    uint16_t _current_time = (uint16_t) current_time;
    uint16_t current_time_buffer = (uint16_t) htons(_current_time);

    uint16_t _total_time = (uint16_t) total_time;
    uint16_t total_time_buffer = (uint16_t) htons(_total_time);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&ID_buffer, sizeof(ID_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&unit_buffer, sizeof(unit_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&current_time_buffer, sizeof(current_time_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&total_time_buffer, sizeof(total_time_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_creation_data(int & creator_ID, int & unit, int & current_time, int & total_time, Socket& client_socket){
    
    int16_t ID_buffer;
    int16_t _ID;

    uint8_t unit_buffer;
    uint8_t _unit;

    uint16_t current_time_buffer;
    uint16_t _current_time;

    uint16_t total_time_buffer;
    uint16_t _total_time;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&ID_buffer, sizeof(ID_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _ID = (int16_t) ntohs(ID_buffer);
    creator_ID = (int) _ID;

    recv_size = client_socket.recvall(&unit_buffer, sizeof(unit_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _unit = (uint8_t) unit_buffer;
    unit = (int) _unit;

    recv_size = client_socket.recvall(&current_time_buffer, sizeof(current_time_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _current_time = (uint16_t) ntohs(current_time_buffer);
    current_time = (int) _current_time;

    recv_size = client_socket.recvall(&total_time_buffer, sizeof(total_time_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _total_time = (uint16_t) ntohs(total_time_buffer);
    total_time = (int) _total_time;

    return;
}

void Protocol::send_creators(int barrack_id, int light_factory_id, int heavy_factory_id, Socket& client_socket) {

    int16_t _barrack_id = (int16_t) barrack_id;
    int16_t barrack_id_buffer = (int16_t) htons(_barrack_id);

    int16_t _light_factory_id = (int16_t) light_factory_id;
    int16_t light_factory_id_buffer = (int16_t)  htons(_light_factory_id);

    int16_t _heavy_factory_id = (int16_t) heavy_factory_id;
    int16_t heavy_factory_id_buffer = (int16_t) htons(_heavy_factory_id);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&barrack_id_buffer, sizeof(barrack_id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&light_factory_id_buffer, sizeof(light_factory_id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&heavy_factory_id_buffer, sizeof(heavy_factory_id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_creators(int& barrack_id, int& light_factory_id, int& heavy_factory_id, Socket& client_socket){
    
    int16_t barrack_id_buffer;
    int16_t _barrack_id;

    int16_t light_factory_id_buffer;
    int16_t _light_factory_id;

    int16_t heavy_factory_id_buffer;
    int16_t _heavy_factory_id;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&barrack_id_buffer, sizeof(barrack_id_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _barrack_id = (int16_t) ntohs(barrack_id_buffer);
    barrack_id = (int) _barrack_id;


    recv_size = client_socket.recvall(&light_factory_id_buffer, sizeof(light_factory_id_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _light_factory_id = (int16_t) ntohs(light_factory_id_buffer);
    light_factory_id = (int) _light_factory_id;


    recv_size = client_socket.recvall(&heavy_factory_id_buffer, sizeof(heavy_factory_id_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _heavy_factory_id = (int16_t) ntohs(heavy_factory_id_buffer);
    heavy_factory_id = (int) _heavy_factory_id;

    return;
}


void Protocol::send_responses_size(int size, Socket& client_socket) {

    uint16_t _size = (uint16_t) size;
    uint16_t size_buffer = (uint16_t) htons(_size);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&size_buffer, sizeof(size_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_responses_size(int& size, Socket& client_socket) {

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

void Protocol::send_creation_data_size(int size, Socket& client_socket) {

    uint16_t _size = (uint16_t) size;
    uint16_t size_buffer = (uint16_t) htons(_size);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&size_buffer, sizeof(size_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_creation_data_size(int& size, Socket& client_socket) {

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


void Protocol::send_selectable_type(selectable_t type, Socket& client_socket) {

    uint8_t _type = (uint8_t) type;

    uint8_t type_buffer = (uint8_t) _type;

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

void Protocol::send_trike(int id, int faction, int lp, int pos_x, int pos_y,int speed,int direction, bool moving, bool selected, bool attacking, int e_pos_x, int e_pos_y, bool waiting, Socket& client_socket) {

    this->send_selectable_type(SEL_TRIKE, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t _speed = (uint8_t) speed;
    uint8_t speed_buffer = (uint8_t) _speed;

    uint8_t _direction = (uint8_t) direction;
    uint8_t direction_buffer = (uint8_t) _direction;

    uint8_t _moving = (uint8_t) moving;
    uint8_t moving_buffer = (uint8_t) _moving;

    uint8_t _attacking = (uint8_t) attacking;
    uint8_t attacking_buffer = (uint8_t) _attacking;

    uint16_t _e_pos_x = (uint16_t) e_pos_x;
    uint16_t e_pos_x_buffer = (uint16_t) htons(_e_pos_x);

    uint16_t _e_pos_y = (uint16_t) e_pos_y;
    uint16_t e_pos_y_buffer = (uint16_t) htons(_e_pos_y);

    uint8_t _waiting = (uint8_t) waiting;
    uint8_t waiting_buffer = (uint8_t) _waiting;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_fremen(int id, int faction, int lp, int pos_x, int pos_y,int speed,int direction, bool moving, bool selected, bool attacking, int e_pos_x, int e_pos_y, bool waiting, Socket& client_socket) {

    this->send_selectable_type(SEL_FREMEN, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t _speed = (uint8_t) speed;
    uint8_t speed_buffer = (uint8_t) _speed;

    uint8_t _direction = (uint8_t) direction;
    uint8_t direction_buffer = (uint8_t) _direction;

    uint8_t _moving = (uint8_t) moving;
    uint8_t moving_buffer = (uint8_t) _moving;

    uint8_t _attacking = (uint8_t) attacking;
    uint8_t attacking_buffer = (uint8_t) _attacking;

    uint16_t _e_pos_x = (uint16_t) e_pos_x;
    uint16_t e_pos_x_buffer = (uint16_t) htons(_e_pos_x);

    uint16_t _e_pos_y = (uint16_t) e_pos_y;
    uint16_t e_pos_y_buffer = (uint16_t) htons(_e_pos_y);

    uint8_t _waiting = (uint8_t) waiting;
    uint8_t waiting_buffer = (uint8_t) _waiting;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_infantry(int id, int faction, int lp, int pos_x, int pos_y,int speed,int direction, bool moving, bool selected, bool attacking, int e_pos_x, int e_pos_y, bool waiting, Socket& client_socket) {

    this->send_selectable_type(SEL_INFANTRY, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t _speed = (uint8_t) speed;
    uint8_t speed_buffer = (uint8_t) _speed;

    uint8_t _direction = (uint8_t) direction;
    uint8_t direction_buffer = (uint8_t) _direction;

    uint8_t _moving = (uint8_t) moving;
    uint8_t moving_buffer = (uint8_t) _moving;

    uint8_t _attacking = (uint8_t) attacking;
    uint8_t attacking_buffer = (uint8_t) _attacking;

    uint16_t _e_pos_x = (uint16_t) e_pos_x;
    uint16_t e_pos_x_buffer = (uint16_t) htons(_e_pos_x);

    uint16_t _e_pos_y = (uint16_t) e_pos_y;
    uint16_t e_pos_y_buffer = (uint16_t) htons(_e_pos_y);

    uint8_t _waiting = (uint8_t) waiting;
    uint8_t waiting_buffer = (uint8_t) _waiting;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}


void Protocol::send_sardaukar(int id, int faction, int lp, int pos_x, int pos_y,int speed,int direction, bool moving, bool selected, bool attacking, int e_pos_x, int e_pos_y, bool waiting, Socket& client_socket) {

    this->send_selectable_type(SEL_SARDAUKAR, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t _speed = (uint8_t) speed;
    uint8_t speed_buffer = (uint8_t) _speed;

    uint8_t _direction = (uint8_t) direction;
    uint8_t direction_buffer = (uint8_t) _direction;

    uint8_t _moving = (uint8_t) moving;
    uint8_t moving_buffer = (uint8_t) _moving;

    uint8_t _attacking = (uint8_t) attacking;
    uint8_t attacking_buffer = (uint8_t) _attacking;

    uint16_t _e_pos_x = (uint16_t) e_pos_x;
    uint16_t e_pos_x_buffer = (uint16_t) htons(_e_pos_x);

    uint16_t _e_pos_y = (uint16_t) e_pos_y;
    uint16_t e_pos_y_buffer = (uint16_t) htons(_e_pos_y);

    uint8_t _waiting = (uint8_t) waiting;
    uint8_t waiting_buffer = (uint8_t) _waiting;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_tank(int id, int faction, int lp, int pos_x, int pos_y,int speed,int direction, bool moving, bool selected, bool attacking, int e_pos_x, int e_pos_y, bool waiting, Socket& client_socket) {

    this->send_selectable_type(SEL_TANK, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t _speed = (uint8_t) speed;
    uint8_t speed_buffer = (uint8_t) _speed;

    uint8_t _direction = (uint8_t) direction;
    uint8_t direction_buffer = (uint8_t) _direction;

    uint8_t _moving = (uint8_t) moving;
    uint8_t moving_buffer = (uint8_t) _moving;

    uint8_t _attacking = (uint8_t) attacking;
    uint8_t attacking_buffer = (uint8_t) _attacking;

    uint16_t _e_pos_x = (uint16_t) e_pos_x;
    uint16_t e_pos_x_buffer = (uint16_t) htons(_e_pos_x);

    uint16_t _e_pos_y = (uint16_t) e_pos_y;
    uint16_t e_pos_y_buffer = (uint16_t) htons(_e_pos_y);

    uint8_t _waiting = (uint8_t) waiting;
    uint8_t waiting_buffer = (uint8_t) _waiting;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_devastator(int id, int faction, int lp, int pos_x, int pos_y,int speed,int direction, bool moving, bool selected, bool attacking, int e_pos_x, int e_pos_y, bool waiting, Socket& client_socket) {

    this->send_selectable_type(SEL_DEVASTATOR, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t _speed = (uint8_t) speed;
    uint8_t speed_buffer = (uint8_t) _speed;

    uint8_t _direction = (uint8_t) direction;
    uint8_t direction_buffer = (uint8_t) _direction;

    uint8_t _moving = (uint8_t) moving;
    uint8_t moving_buffer = (uint8_t) _moving;

    uint8_t _attacking = (uint8_t) attacking;
    uint8_t attacking_buffer = (uint8_t) _attacking;

    uint16_t _e_pos_x = (uint16_t) e_pos_x;
    uint16_t e_pos_x_buffer = (uint16_t) htons(_e_pos_x);

    uint16_t _e_pos_y = (uint16_t) e_pos_y;
    uint16_t e_pos_y_buffer = (uint16_t) htons(_e_pos_y);

    uint8_t _waiting = (uint8_t) waiting;
    uint8_t waiting_buffer = (uint8_t) _waiting;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);
    
    sent_size = client_socket.sendall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::send_harvester(int id, int faction, int lp, int pos_x, int pos_y,int speed,int direction,bool moving, bool selected, int spice, bool harvesting,bool waiting, Socket& client_socket) {

    this->send_selectable_type(SEL_HARVESTER, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t _speed = (uint8_t) speed;
    uint8_t _direction = (uint8_t) direction;
    uint8_t _moving = (uint8_t) moving;
    uint16_t _spice = (uint16_t) spice;
    uint8_t _harvesting = (uint8_t) harvesting;
    uint8_t _waiting = (uint8_t) waiting;

    uint8_t speed_buffer = (uint8_t) _speed;
    uint8_t direction_buffer = (uint8_t) _direction;
    uint8_t moving_buffer = (uint8_t) _moving;
    uint16_t spice_buffer = (uint16_t) htons(_spice);
    uint8_t harvesting_buffer = (uint8_t) _harvesting;
    uint8_t waiting_buffer = (uint8_t) _waiting;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&harvesting_buffer, sizeof(harvesting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;

}

void Protocol::send_construction_yard(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_CONSTRUCTION_YARD, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::send_air_trap(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_AIR_TRAP, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::send_barrack(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_BARRACK, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::send_light_factory(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_LIGHT_FACTORY, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::send_heavy_factory(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_HEAVY_FACTORY, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::send_silo(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_SILO, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::send_palace(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_PALACE, client_socket);

    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::send_refinery(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    this->send_selectable_type(SEL_REFINERY, client_socket);
    this->send_element(id, faction, lp, pos_x, pos_y, selected, client_socket);
}

void Protocol::receive_trike(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int& speed, int& dir, bool& moving,bool& selected, bool& attacking, int& e_pos_x, int& e_pos_y, bool & waiting, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t speed_buffer;
    uint8_t _speed;

    uint8_t direction_buffer;
    uint8_t _direction;

    uint8_t moving_buffer;
    uint8_t _moving;

    uint8_t attacking_buffer;
    uint8_t _attacking;

    uint16_t e_pos_x_buffer;
    uint16_t _e_pos_x;

    uint16_t e_pos_y_buffer;
    uint16_t _e_pos_y;

    uint8_t waiting_buffer;
    uint8_t _waiting;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _speed = (uint8_t) speed_buffer;
    speed = (int) _speed;

    recv_size = client_socket.recvall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _direction = (uint8_t) direction_buffer;
    dir = (int) _direction;

    recv_size = client_socket.recvall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _moving = (uint8_t) moving_buffer;
    moving = (bool) _moving;

    recv_size = client_socket.recvall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _attacking = (uint8_t) attacking_buffer;
    attacking = (bool) _attacking;

    recv_size = client_socket.recvall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_x = (uint16_t) ntohs(e_pos_x_buffer);
    e_pos_x = (int) _e_pos_x;

    recv_size = client_socket.recvall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_y = (uint16_t) ntohs(e_pos_y_buffer);
    e_pos_y = (int) _e_pos_y;

    recv_size = client_socket.recvall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _waiting = (uint8_t) waiting_buffer;
    waiting = (bool) _waiting;

    return;
}

void Protocol::receive_fremen(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int& speed, int& dir, bool& moving,bool& selected, bool& attacking, int& e_pos_x, int& e_pos_y, bool & waiting, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t speed_buffer;
    uint8_t _speed;

    uint8_t direction_buffer;
    uint8_t _direction;

    uint8_t moving_buffer;
    uint8_t _moving;

    uint8_t attacking_buffer;
    uint8_t _attacking;

    uint16_t e_pos_x_buffer;
    uint16_t _e_pos_x;

    uint16_t e_pos_y_buffer;
    uint16_t _e_pos_y;

    uint8_t waiting_buffer;
    uint8_t _waiting;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _speed = (uint8_t) speed_buffer;
    speed = (int) _speed;

    recv_size = client_socket.recvall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _direction = (uint8_t) direction_buffer;
    dir = (int) _direction;

    recv_size = client_socket.recvall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _moving = (uint8_t) moving_buffer;
    moving = (bool) _moving;

    recv_size = client_socket.recvall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _attacking = (uint8_t) attacking_buffer;
    attacking = (bool) _attacking;

    recv_size = client_socket.recvall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_x = (uint16_t) ntohs(e_pos_x_buffer);
    e_pos_x = (int) _e_pos_x;

    recv_size = client_socket.recvall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_y = (uint16_t) ntohs(e_pos_y_buffer);
    e_pos_y = (int) _e_pos_y;

    recv_size = client_socket.recvall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _waiting = (uint8_t) waiting_buffer;
    waiting = (bool) _waiting;

    return;
}

void Protocol::receive_infantry(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int& speed, int& dir, bool& moving,bool& selected, bool& attacking, int& e_pos_x, int& e_pos_y, bool & waiting, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t speed_buffer;
    uint8_t _speed;

    uint8_t direction_buffer;
    uint8_t _direction;

    uint8_t moving_buffer;
    uint8_t _moving;

    uint8_t attacking_buffer;
    uint8_t _attacking;

    uint16_t e_pos_x_buffer;
    uint16_t _e_pos_x;

    uint16_t e_pos_y_buffer;
    uint16_t _e_pos_y;

    uint8_t waiting_buffer;
    uint8_t _waiting;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _speed = (uint8_t) speed_buffer;
    speed = (int) _speed;

    recv_size = client_socket.recvall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _direction = (uint8_t) direction_buffer;
    dir = (int) _direction;

    recv_size = client_socket.recvall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _moving = (uint8_t) moving_buffer;
    moving = (bool) _moving;

    recv_size = client_socket.recvall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _attacking = (uint8_t) attacking_buffer;
    attacking = (bool) _attacking;

    recv_size = client_socket.recvall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_x = (uint16_t) ntohs(e_pos_x_buffer);
    e_pos_x = (int) _e_pos_x;

    recv_size = client_socket.recvall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_y = (uint16_t) ntohs(e_pos_y_buffer);
    e_pos_y = (int) _e_pos_y;

    recv_size = client_socket.recvall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _waiting = (uint8_t) waiting_buffer;
    waiting = (bool) _waiting;

    return;
}

void Protocol::receive_sardaukar(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int& speed, int& dir, bool& moving,bool& selected, bool& attacking, int& e_pos_x, int& e_pos_y, bool & waiting, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t speed_buffer;
    uint8_t _speed;

    uint8_t direction_buffer;
    uint8_t _direction;

    uint8_t moving_buffer;
    uint8_t _moving;

    uint8_t attacking_buffer;
    uint8_t _attacking;

    uint16_t e_pos_x_buffer;
    uint16_t _e_pos_x;

    uint16_t e_pos_y_buffer;
    uint16_t _e_pos_y;

    uint8_t waiting_buffer;
    uint8_t _waiting;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _speed = (uint8_t) speed_buffer;
    speed = (int) _speed;

    recv_size = client_socket.recvall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _direction = (uint8_t) direction_buffer;
    dir = (int) _direction;

    recv_size = client_socket.recvall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _moving = (uint8_t) moving_buffer;
    moving = (bool) _moving;

    recv_size = client_socket.recvall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _attacking = (uint8_t) attacking_buffer;
    attacking = (bool) _attacking;

    recv_size = client_socket.recvall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_x = (uint16_t) ntohs(e_pos_x_buffer);
    e_pos_x = (int) _e_pos_x;

    recv_size = client_socket.recvall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_y = (uint16_t) ntohs(e_pos_y_buffer);
    e_pos_y = (int) _e_pos_y;

    recv_size = client_socket.recvall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _waiting = (uint8_t) waiting_buffer;
    waiting = (bool) _waiting;

    return;
}

void Protocol::receive_tank(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int& speed, int& dir, bool& moving,bool& selected, bool& attacking, int& e_pos_x, int& e_pos_y, bool & waiting, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t speed_buffer;
    uint8_t _speed;

    uint8_t direction_buffer;
    uint8_t _direction;

    uint8_t moving_buffer;
    uint8_t _moving;

    uint8_t attacking_buffer;
    uint8_t _attacking;

    uint16_t e_pos_x_buffer;
    uint16_t _e_pos_x;

    uint16_t e_pos_y_buffer;
    uint16_t _e_pos_y;

    uint8_t waiting_buffer;
    uint8_t _waiting;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _speed = (uint8_t) speed_buffer;
    speed = (int) _speed;

    recv_size = client_socket.recvall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _direction = (uint8_t) direction_buffer;
    dir = (int) _direction;

    recv_size = client_socket.recvall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _moving = (uint8_t) moving_buffer;
    moving = (bool) _moving;

    recv_size = client_socket.recvall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _attacking = (uint8_t) attacking_buffer;
    attacking = (bool) _attacking;

    recv_size = client_socket.recvall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_x = (uint16_t) ntohs(e_pos_x_buffer);
    e_pos_x = (int) _e_pos_x;

    recv_size = client_socket.recvall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_y = (uint16_t) ntohs(e_pos_y_buffer);
    e_pos_y = (int) _e_pos_y;

    recv_size = client_socket.recvall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _waiting = (uint8_t) waiting_buffer;
    waiting = (bool) _waiting;

    return;
}
void Protocol::receive_devastator(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int& speed, int& dir, bool& moving,bool& selected, bool& attacking, int& e_pos_x, int& e_pos_y, bool & waiting, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t speed_buffer;
    uint8_t _speed;

    uint8_t direction_buffer;
    uint8_t _direction;

    uint8_t moving_buffer;
    uint8_t _moving;

    uint8_t attacking_buffer;
    uint8_t _attacking;

    uint16_t e_pos_x_buffer;
    uint16_t _e_pos_x;

    uint16_t e_pos_y_buffer;
    uint16_t _e_pos_y;

    uint8_t waiting_buffer;
    uint8_t _waiting;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _speed = (uint8_t) speed_buffer;
    speed = (int) _speed;

    recv_size = client_socket.recvall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _direction = (uint8_t) direction_buffer;
    dir = (int) _direction;

    recv_size = client_socket.recvall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _moving = (uint8_t) moving_buffer;
    moving = (bool) _moving;

    recv_size = client_socket.recvall(&attacking_buffer, sizeof(attacking_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _attacking = (uint8_t) attacking_buffer;
    attacking = (bool) _attacking;

    recv_size = client_socket.recvall(&e_pos_x_buffer, sizeof(e_pos_x_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_x = (uint16_t) ntohs(e_pos_x_buffer);
    e_pos_x = (int) _e_pos_x;

    recv_size = client_socket.recvall(&e_pos_y_buffer, sizeof(e_pos_y_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _e_pos_y = (uint16_t) ntohs(e_pos_y_buffer);
    e_pos_y = (int) _e_pos_y;

    recv_size = client_socket.recvall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _waiting = (uint8_t) waiting_buffer;
    waiting = (bool) _waiting;

    return;
}
void Protocol::receive_harvester(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int& speed, int & direction,bool & moving,bool& selected, int& spice, bool& harvesting,bool & waiting, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    uint8_t speed_buffer;
    uint8_t _speed;
    uint8_t direction_buffer;
    uint8_t _direction;
    uint8_t moving_buffer;
    uint8_t _moving;
    uint16_t spice_buffer;
    uint16_t _spice;
    uint8_t harvesting_buffer;
    uint8_t _harvesting;
    uint8_t waiting_buffer;
    uint8_t _waiting;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&speed_buffer, sizeof(speed_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _speed = (uint8_t) speed_buffer;
    speed = (int) _speed;
    recv_size = client_socket.recvall(&direction_buffer, sizeof(direction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _direction = (uint8_t) direction_buffer;
    recv_size = client_socket.recvall(&moving_buffer, sizeof(moving_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _moving = (uint8_t) moving_buffer;
    recv_size = client_socket.recvall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _spice = (uint16_t) ntohs(spice_buffer);
    recv_size = client_socket.recvall(&harvesting_buffer, sizeof(harvesting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _harvesting = (uint8_t) harvesting_buffer;
    recv_size = client_socket.recvall(&waiting_buffer, sizeof(waiting_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _waiting = (uint8_t) waiting_buffer;

    direction = (int) _direction;
    moving = (bool) _moving;
    spice = (int) _spice;
    harvesting = (bool) _harvesting;
    waiting = (bool) _waiting;

    return;
}

void Protocol::receive_construction_yard(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}


void Protocol::receive_air_trap(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::receive_barrack(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::receive_light_factory(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::receive_heavy_factory(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::receive_silo(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::receive_palace(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);

    return;
}

void Protocol::receive_refinery(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    this->receive_element(id, faction, lp, pos_x, pos_y, selected, client_socket);    
    return;
}

void Protocol::send_element(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket) {

    uint16_t _id = (uint16_t) id;
    uint8_t _faction = (uint8_t) faction;
    uint16_t _lp = (uint16_t) lp;
    uint16_t _pos_x = (uint16_t) pos_x;
    uint16_t _pos_y = (uint16_t) pos_y;
    uint8_t _selected = (uint8_t) selected;

    uint16_t id_buffer = (uint16_t) htons(_id);
    uint8_t faction_buffer = (uint8_t) _faction;
    uint16_t lp_buffer = (uint16_t) htons(_lp);
    uint16_t pos_x_buffer = (uint16_t) htons(_pos_x);
    uint16_t pos_y_buffer = (uint16_t) htons(_pos_y);
    uint8_t selected_buffer = (uint8_t) _selected;

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&faction_buffer, sizeof(faction_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&lp_buffer, sizeof(lp_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_x_buffer, sizeof(pos_x_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&pos_y_buffer, sizeof(pos_y_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&selected_buffer, sizeof(selected_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;    

}

void Protocol::receive_element(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket) {

    uint16_t id_buffer;
    uint8_t faction_buffer;
    uint16_t lp_buffer;
    uint16_t pos_x_buffer;
    uint16_t pos_y_buffer;
    uint8_t selected_buffer;

    uint16_t _id;
    uint8_t _faction;
    uint16_t _lp;
    uint16_t _pos_x;
    uint16_t _pos_y;
    uint8_t _selected;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&id_buffer, sizeof(id_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _id = (uint16_t) ntohs(id_buffer);

    recv_size = client_socket.recvall(&faction_buffer, sizeof(faction_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _faction = (uint8_t) faction_buffer;

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
    faction = (int) _faction;
    lp = (int) _lp;
    pos_x = (int) _pos_x;
    pos_y = (int) _pos_y;
    selected = (bool) _selected;

    return;
}

void Protocol::send_selectables_to_read(int size, Socket& client_socket) {

    uint16_t _size = (uint16_t) size;
    uint16_t size_buffer = (uint16_t) htons(_size);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&size_buffer, sizeof(size_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;
}

void Protocol::receive_selectables_to_read(int& size, Socket& client_socket) {

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

void Protocol::send_player_state(int spice, int c_spice,int energy, Socket& client_socket) {

    uint16_t _spice = (uint16_t) spice;
    uint16_t _c_spice = (uint16_t) c_spice;
    int16_t _energy = (int16_t) energy;

    uint16_t spice_buffer = (uint16_t) htons(_spice);
    uint16_t c_spice_buffer = (uint16_t) htons(_c_spice);
    int16_t energy_buffer = (int16_t) htons(_energy);

    int sent_size = -1;
    bool was_closed = false;

    sent_size = client_socket.sendall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&c_spice_buffer, sizeof(c_spice_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    sent_size = client_socket.sendall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_dispatch(was_closed, sent_size);

    return;

}
void Protocol::receive_player_state(int& spice,int& c_spice, int& energy, Socket& client_socket) {

    uint16_t spice_buffer;
    uint16_t c_spice_buffer;
    int16_t energy_buffer;

    uint16_t _spice;
    uint16_t _c_spice;
    int16_t _energy;

    int recv_size = -1;
    bool was_closed = false;

    recv_size = client_socket.recvall(&spice_buffer, sizeof(spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _spice = (uint16_t) ntohs(spice_buffer);

    recv_size = client_socket.recvall(&c_spice_buffer, sizeof(c_spice_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _c_spice = (uint16_t) ntohs(c_spice_buffer);

    recv_size = client_socket.recvall(&energy_buffer, sizeof(energy_buffer), &was_closed);
    handle_receive(was_closed, recv_size);
    _energy = (int16_t) ntohs(energy_buffer);

    spice = (int) _spice;
    c_spice = (int) _c_spice;
    energy = (int) _energy;

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