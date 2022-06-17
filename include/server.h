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
    double efficiency;
    std::map<int,std::unique_ptr<Selectable>> elements;
    Board board;
    std::queue<std::vector<int>> mouse_events;
    std::vector<State> states;
    bool place(Building& building,Position position);
    bool place(Refinery& refinery,Position& position);
    bool place(Unit& unit,std::vector<Position> positions);
 public:    
   Server (std::vector<std::vector<cell_t>> cells);
   void print();
   void run(player_t _faction, int _spice, int _c_spice, int _energy, int _c_energy, CPlayer& _client_player); // This should receive the socket in the future
   void update();
   void createBuilding(int type, int pos_x, int pos_y, int& spice, int& c_spice, int& energy, int& c_energy, CPlayer& client_player);
   void createUnit(int type, int& spice, CPlayer& client_player);
   void handleLeftClick(int x, int y, CPlayer& client_player);
   void handleRightClick(int x, int y, CPlayer& client_player);
   void handleSelection(int xmin, int xmax, int ymin, int ymax, CPlayer& client_player);
   void reportStateToClient(CPlayer& client_player, int spice, int energy);
};
