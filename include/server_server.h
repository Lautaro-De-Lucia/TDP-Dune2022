#pragma once

#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <unistd.h>
#include <memory>

#include "server_selectable.h"
#include "server_buildingfactory.h"
#include "server_unitfactory.h"
#include "server_unit.h"

#include "common_utils.h"
#include "common_socket.h"
#include "common_protocol.h"

#define INSTRUCTIONS_FILE "/instr.txt"

class Server {
 private:
   Socket socket;
   Socket client_socket;
   Protocol protocol;
   double efficiency;
   std::map<int,std::unique_ptr<Selectable>> elements;
   Board board;
   std::vector<State> states;
   bool place(Building& building,Position position);
   bool place(Refinery& refinery,Position& position);
   bool place(Unit& unit,std::vector<Position> positions);
 public:    
   Server (const char* service_name, std::vector<std::vector<cell_t>> cells);
   void print();
   void run(player_t _faction, int _spice, int _c_spice, int _energy, int _c_energy); // This should receive the socket in the future
   void update();
   response_t createBuilding(int type, int pos_x, int pos_y, int& spice, int& c_spice, int& energy, int& c_energy);
   response_t createUnit(int type, int& spice);
   response_t handleLeftClick(int x, int y);
   response_t handleRightClick(int x, int y);
   response_t handleSelection(int xmin, int xmax, int ymin, int ymax);
   void send_element(Trike& trike, int __id);
   void send_element(Harvester& harvester, int __id);
   void send_element(AirTrap& air_trap, int __id);
   void send_element(Barrack& barrack, int __id);
   void send_element(Refinery& refinery, int __id);
   void reportStateToClient(int spice, int energy);
};
