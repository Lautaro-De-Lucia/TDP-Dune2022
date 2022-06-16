#pragma once

#include <vector>
#include <string>

#include "socket.h"
#include "common_utils.h"
#include "selectable.h"
#include "client_selectable.h"

class Protocol {
 private:
 public:
    Protocol();
    ~Protocol();

    void handle_dispatch(bool was_closed, int sent_size);
    void handle_receive(bool was_closed, int recv_size);

    // client to server requests
    void send_create_building_request(int type, int pos_x, int pos_y, Socket& client_socket);
    void send_create_unit_request(int type, Socket& client_socket);
    void send_mouse_left_click(int pos_x, int pos_y, Socket& client_socket);
    void send_mouse_right_click(int pos_x, int pos_y, Socket& client_socket);
    void send_mouse_selection(int pos_x_min, int pos_x_max, int pos_y_min, int pos_y_max, Socket& client_socket);
    void send_idle(Socket& client_socket);

    // server side requests receiver
    void receive_command(command_t& command, Socket& client_socket);
    void receive_create_building_request(int& type, int& pos_x, int& pos_y, Socket& client_socket);
    void receive_create_unit_request(int& type, Socket& client_socket);
    void receive_mouse_left_click(int& pos_x, int& pos_y, Socket& client_socket);
    void receive_mouse_right_click(int& pos_x, int& pos_y, Socket& client_socket);
    void receive_mouse_selection(int& pos_x_min, int& pos_x_max, int& pos_y_min, int& pos_y_max, Socket& client_socket);

    // server to client requests responses
    void send_command_response(response_t response, Socket& client_socket);

    // client side command status receiver
    void receive_command_response(response_t& response, Socket& client_socket);

    // server to client game state report
    void send_game_state(const std::map<int, std::unique_ptr<Selectable>>& elements, Socket& client_socket);

    // client side game state receiver
    void receive_game_state(std::map<int, std::unique_ptr<CSelectable>>& elements, Socket& client_socket);
};
