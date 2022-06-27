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

    // generic element send/receive
    void send_element(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void receive_element(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);

 public:

    Protocol();
    ~Protocol();

    // faction request
    void send_faction_request(int faction, Socket& client_socket);
    void receive_faction_request(int& faction, Socket& client_socket);
    void send_faction_request_response(bool success, Socket& client_socket);
    void receive_faction_request_response(bool& success, Socket& client_socket);

    // game start
    void send_game_has_started(Socket& client_socket);
    void receive_game_has_started(Socket& client_socket);

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
    void send_response(response_t response, Socket& client_socket);
    void send_responses_size(int responses_size, Socket& client_socket);

    // client side command status receiver
    void receive_response(response_t& response, Socket& client_socket);
    void receive_responses_size(int& responses_size, Socket& client_socket);

    // single send/receive type of selectable
    void send_selectable_type(selectable_t type, Socket& client_socket);
    void receive_selectable_type(selectable_t& type, Socket& client_socket);

    // send each type of unit & building
    void send_trike(int id, int faction, int lp, int pos_x, int pos_y,int direction, bool moving,bool selected, bool attacking, int e_pos_x, int e_pos_y, bool waiting, Socket& client_socket);
    void send_fremen(int id, int faction, int lp, int pos_x, int pos_y, bool selected, bool attacking, Socket& client_socket);
    void send_infantry(int id, int faction, int lp, int pos_x, int pos_y, bool selected, bool attacking, Socket& client_socket);
    void send_sardaukar(int id, int faction, int lp, int pos_x, int pos_y, bool selected, bool attacking, Socket& client_socket);
    void send_tank(int id, int faction, int lp, int pos_x, int pos_y, bool selected, bool attacking, Socket& client_socket);
    void send_devastator(int id, int faction, int lp, int pos_x, int pos_y, bool selected, bool attacking, Socket& client_socket);

    void send_construction_yard(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_harvester(int id, int faction, int lp, int pos_x, int pos_y,int direction, bool moving,bool selected, int spice, bool harvesting,bool waiting, Socket& client_socket);
    void send_air_trap(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_barrack(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_refinery(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_light_factory(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_heavy_factory(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_silo(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
    void send_palace(int id, int faction, int lp, int pos_x, int pos_y, bool selected, Socket& client_socket);
 
    // receive each type of unit & building
    void receive_trike(int& id, int& faction, int& lp, int& pos_x, int& pos_y, int& direction,bool& moving,bool& selected, bool& attacking, int& e_pos_x, int& e_pos_y, bool & waiting, Socket& client_socket);
    void receive_fremen(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket);
    void receive_infantry(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket);
    void receive_sardaukar(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket);
    void receive_tank(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket);
    void receive_devastator(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, bool& attacking, Socket& client_socket);

    void receive_construction_yard(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_harvester(int& id, int& faction, int& lp, int& pos_x, int& pos_y,int & direction,bool& moving, bool& selected, int& spice, bool& harvesting, bool & waiting, Socket& client_socket);
    void receive_air_trap(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_barrack(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_refinery(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_light_factory(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_heavy_factory(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_silo(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);
    void receive_palace(int& id, int& faction, int& lp, int& pos_x, int& pos_y, bool& selected, Socket& client_socket);

    // communicate selectables collection size
    void send_selectables_to_read(int size, Socket& client_socket);
    void receive_selectables_to_read(int& size, Socket& client_socket);

    // sand cells communication
    void send_sand_cells_size(int size, Socket& client_socket);
    void receive_sand_cells_size(int& size, Socket& client_socket);
    void send_sand_cell(int pos_x, int pos_y, int spice, Socket& client_socket);
    void receive_sand_cell(int& pos_x, int& pos_y, int& spice, Socket& client_socket);

    // player state send/receive
    void send_player_state(int spice, int c_spice,int energy, Socket& client_socket);
    void receive_player_state(int& spice, int& c_spice,int& energy, Socket& client_socket);
};
