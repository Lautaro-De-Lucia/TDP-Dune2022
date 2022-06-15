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

class Player {
 private:
    int ID;
    int spice;
    int c_spice;
    int energy;
    int c_energy;
    player_t faction;
    double efficiency;
    CPlayer& cplayer;
    std::map<int,std::unique_ptr<Selectable>> elements;
    Board board;
    std::queue<std::vector<int>> mouse_events;
    std::vector<State> states;
    bool place(Building& building,Position position);
    bool place(Refinery& refinery,Position& position);
    bool place(Unit& unit,std::vector<Position> positions);
 public:    
    Player (player_t faction ,int spice, int c_spice, int energy, int c_energy,std::vector<std::vector<cell_t>> cell_types,CPlayer& client_player);
    void print();
    void run(); // This should receive the socket in the future
    void update();
    void createBuilding(int type, int pos_x, int pos_y);
    void createUnit(int type);
    void handleLeftClick(int x, int y);
    void handleRightClick(int x, int y);
    void handleSelection(int xmin, int xmax, int ymin, int ymax);
    void handleIdle();
    void reportStateToClient();
};
