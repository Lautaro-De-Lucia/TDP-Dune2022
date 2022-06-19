#pragma once

#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <thread>
#include <unistd.h>
#include <memory>

#include "server_selectable.h"
#include "server_buildingfactory.h"
#include "server_unitfactory.h"
#include "server_unit.h"
#include "server_gameresources.h"

#include "common_utils.h"
#include "common_socket.h"
#include "common_protocol.h"

class ClientHandler {
 private:
   player_t faction;
   int spice;
   int max_spice;
   int energy;
   int max_energy;
   int efficiency;
   Protocol protocol;
   GameResources * game;
   bool finished;
   std::thread thread;

 public:    
   ClientHandler (player_t faction,int init_energy, int init_spice ,Socket & client_socket,GameResources * game);
   void run(player_t _faction, int _spice, int _c_spice, int _energy, int _c_energy); // This should receive the socket in the future
   response_t createBuilding(int type, int pos_x, int pos_y, int& spice, int& c_spice, int& energy, int& c_energy);
   response_t createUnit(int type, int& spice);
   response_t handleLeftClick(int x, int y);
   response_t handleRightClick(int x, int y);
   response_t handleSelection(int xmin, int xmax, int ymin, int ymax);
   void reportState();
};
