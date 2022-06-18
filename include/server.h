#pragma once

#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <unistd.h>
#include <memory>

#include "textfilehandler.h"
#include "selectable.h"
#include "buildingfactory.h"
#include "unitfactory.h"
#include "unit.h"
#include "client_player.h"
#include "common_utils.h"

#define INSTRUCTIONS_FILE "/instr.txt"

class Server {
 private:
   Socket socket;
   Socket client_socket;
   Protocol protocol;
   double efficiency;
   std::map<int,std::unique_ptr<Selectable>> elements;
   Board board;
   std::queue<std::vector<int>> mouse_events;
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
   void reportStateToClient(CPlayer& client_player, int spice, int energy);
};
