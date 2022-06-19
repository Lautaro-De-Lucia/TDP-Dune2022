#pragma once

#include <vector>
#include <string>

#include "common_utils.h"
#include "common_socket.h"

class Protocol {
 private:

    // exception throwers
    void handle_dispatch(bool was_closed, int sent_size);
    void handle_receive(bool was_closed, int recv_size);

    // generic element receiver
    void receive_element(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);

 public:

    Protocol();
    ~Protocol();

    // client to server requests
    void send_command(command_t command, Socket& client_socket);
    void send_create_building_request(int type, int pos_x, int pos_y, Socket& client_socket);
    void send_create_unit_request(int type, Socket& client_socket);
    void send_mouse_left_click(int pos_x, int pos_y, Socket& client_socket);
    void send_mouse_right_click(int pos_x, int pos_y, Socket& client_socket);
    void send_mouse_selection(int pos_x_min, int pos_x_max, int pos_y_min, int pos_y_max, Socket& client_socket);

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

    // single send/receive type of selectable
    void send_selectable_type(selectable_t type, Socket& client_socket);
    void receive_selectable_type(selectable_t& type, Socket& client_socket);

    // send each type of unit & building
    void send_trike(int id, int lp, int pos_x, int pos_y, bool selected, bool attacking, Socket& client_socket);
    void send_harvester(int id, int lp, int pos_x, int pos_y, bool selected, int spice, bool harvesting, Socket& client_socket);
    void send_air_trap(int id, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_barrack(int id, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_refinery(int id, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);

    // receive each type of unit & building
    void receive_trike(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket);
    void receive_harvester(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, int& spice, bool& harvesting, Socket& client_socket);
    void receive_air_trap(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_barrack(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_refinery(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);

    // communicate selectables collection size
    void send_selectables_to_read(int size, Socket& client_socket);
    void receive_selectables_to_read(int& size, Socket& client_socket);

    // sand cells communication
    void send_sand_cells_size(int size, Socket& client_socket);
    void receive_sand_cells_size(int& size, Socket& client_socket);
    void send_sand_cell(int pos_x, int pos_y, int spice, Socket& client_socket);
    void receive_sand_cell(int& pos_x, int& pos_y, int& spice, Socket& client_socket);

    // player state send/receive
    void send_player_state(int spice, int energy, Socket& client_socket);
    void receive_player_state(int& spice, int& energy, Socket& client_socket);
};
