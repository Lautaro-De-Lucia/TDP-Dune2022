#pragma once

#include <vector>
#include <string>

#include "socket.h"
#include "common_utils.h"

class Protocol {
 private:
 public:
    Protocol();
    ~Protocol();
    
    // return some status_t?
    void send_create_building_request(int type, int pos_x, int pos_y, Socket& client_socket);
    void send_create_unit_request(int type, Socket& client_socket);
    void send_mouse_left_click(int pos_x, int pos_y, Socket& client_socket);
    void send_mouse_right_click(int pos_x, int pos_y, Socket& client_socket);
    void send_mouse_selection(int pos_x_min, int pos_x_max, int pos_y_min, int pos_y_max, Socket& client_socket);
    void send_idle(Socket& client_socket);







    void handle_dispatch(bool was_closed, int sent_size);
    void handle_receive(bool was_closed, int recv_size);
};

