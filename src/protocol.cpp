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
    uint16_t _pos_x_max= (uint16_t) pos_x_max;
    uint16_t _pos_y_min = (uint16_t) pos_y_min;
    uint16_t _pos_y_max= (uint16_t) pos_y_max;

    uint8_t selection_command_buffer = (uint8_t) selection_command - '0';
    uint16_t pos_x_min_buffer = (uint16_t) htons(pos_x_min);
    uint16_t pos_x_max_buffer = (uint16_t) htons(pos_x_max);
    uint16_t pos_y_min_buffer = (uint16_t) htons(pos_y_min);
    uint16_t pos_y_max_buffer = (uint16_t) htons(pos_y_max);

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

    int sent_size = -1;
    bool was_closed = false;

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

    recv_size = client_socket.recvall(&_type, sizeof(_type), &was_closed);
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

    recv_size = client_socket.recvall(&_type, sizeof(_type), &was_closed);
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
