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
#include "trikefactory.h"

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
    uint16_t ID;
    uint16_t spice;
    uint16_t c_spice;
    int energy;
    uint16_t c_energy;
    double efficiency;
    std::map<uint16_t,std::unique_ptr<Selectable>> elements;
    std::map<unit_t,uint16_t> creators;
    bool place(Building & building,Position position);
    bool place(Refinery & refinery,Position & position);
    bool place(Trike & unit,std::vector<Position> positions);
public:    
    Player (uint16_t spice, uint16_t c_spice, uint16_t energy, uint16_t c_energy);
    void makeCreator();
    void print();
    void run(); // This should receive the socket in the future
    void createUnit();
    void createBuilding();
    void handleLeftClick();
    void handleRightClick();
    void handleSelection();
    void handleIdle();
    void reportState();
    void updateState();
};


