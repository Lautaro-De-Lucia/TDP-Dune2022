#pragma once

#include <vector>
#include <string>

#include "socket.h"
#include "common_utils.h"
#include "selectable.h"
#include "unit.h"
#include "building.h"
#include "client_selectable.h"
#include "client_gamemap.h"

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

    // single send/receive type of selectable
    void send_selectable_type(selectable_t type, Socket& client_socket);
    void receive_selectable_type(selectable_t& type, Socket& client_socket);

    // send each type of unit & building
    void send_element(Trike& trike, int ID, Socket& client_socket);
    void send_element(Harvester& harvester, int ID, Socket& client_socket);
    void send_element(AirTrap& air_trap, int ID, Socket& client_socket);
    void send_element(Barrack& barrack, int ID, Socket& client_socket);
    void send_element(Refinery& refinery, int ID, Socket& client_socket);

    // receive each type of unit & building
    void receive_trike(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket);
    void receive_harvester(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, int& spice, bool& harvesting, Socket& client_socket);
    void receive_air_trap(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_barrack(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_refinery(int& id, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);

    // communicate selectables collection size
    void send_selectables_size(int size, Socket& client_socket);
    void receive_selectables_size(int& size, Socket& client_socket);

    // sand cells communication
    void send_sand_cells_size(int size, Socket& client_socket);
    void receive_sand_cells_size(int& size, Socket& client_socket);
    void send_sand_cell(int pos_x, int pos_y, int spice, Socket& client_socket);
    void receive_sand_cell(int& pos_x, int& pos_y, int& spice, Socket& client_socket);
};
