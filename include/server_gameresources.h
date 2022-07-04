#pragma once

#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <unistd.h>
#include <memory>
#include <mutex>

#include "server_selectable.h"
#include "server_buildingfactory.h"
#include "server_unitfactory.h"
#include "server_unit.h"

#include "common_utils.h"
#include "common_socket.h"
#include "common_protocol.h"

class GameResources {
    std::map<int,std::unique_ptr<Selectable>> elements;
    Board board;
    std::mutex lock;
public:    
    GameResources(std::vector<std::vector<cell_t>> cells);

    response_t createUnit(player_t faction,unit_t type,int & spice);
    response_t createBuilding(player_t faction,building_t type,int pos_x,int pos_y,int & spice,int &energy);
    void selectElement(player_t faction,int pos_x, int pos_y);
    void selectElements(player_t faction,int Xmin, int Xmax,int Ymin, int Ymax);
    void reactToPosition(player_t faction, int pos_x, int pos_y);
    int totalElements();
    void sendElements(Protocol & protocol, Socket & client_socket);
    void sendCreators(player_t faction,Protocol & protocol, Socket & client_socket);
    void sendCreationData(std::vector<creation_t> & creation_data,Protocol & protocol, Socket & client_socket);


    int getSpiceAt(int x, int y);

    void update();

    Cell& getCell(int x, int y);
    int getCreator(player_t faction, unit_t type);
    std::unique_ptr<Selectable>& getElementAt(int x, int y);
    bool isEnabled(player_t faction,unit_t unit);
    bool hasLost(player_t faction);
    int getTotalChangedCells();
    void deleteElement(int ID);
    std::vector<Position> getChangedCells();
    int getSpiceCapacity(player_t faction);
    int getTotalEnergy(player_t faction);
    int getTotalCreators(player_t faction, unit_t type);
    void clearChangedCells();
};
