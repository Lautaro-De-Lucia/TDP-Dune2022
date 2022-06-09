#pragma once

#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include <unistd.h>
#include <memory>

#include "textfilehandler.h"
#include "selectable.h"
#include "buildingfactory.h"
#include "unitfactory.h"
#include "unit.h"
#include "client_player.h"

#define INSTRUCTIONS_FILE "/instr.txt"

enum command_t {
    CREATE_BUILDING = 1,
    CREATE_UNIT = 2, 
    MAKE_CREATOR = 3,
    MOUSE_LEFT_CLICK = 4,
    MOUSE_RIGHT_CLICK = 5,
    MOUSE_SELECTION = 6,
    TEST_ASTAR = 7,
    IDLE = 8,
};

class Player {
 private:
    int ID;
    int spice;
    int c_spice;
    int energy;
    int c_energy;
    double efficiency;
    CPlayer& cplayer;
    std::map<int,std::unique_ptr<Selectable>> elements;
    std::map<unit_t,int> creators;
    bool place(Building & building,Position position);
    bool place(Refinery & refinery,Position & position);
    bool place(Unit & unit,std::vector<Position> positions);
 public:    
    Player (int spice, int c_spice, int energy, int c_energy, CPlayer& client_player);
    void makeCreator();
    void print();
    void run(); // This should receive the socket in the future
    void createBuilding();
    void createUnit();
    void handleLeftClick();
    void handleRightClick();
    void handleSelection();
    void handleIdle();
    void reportState();
    void updateMovables();
};
