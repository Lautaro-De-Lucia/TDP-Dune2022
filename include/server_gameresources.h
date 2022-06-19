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
    Cell& getCell(int x, int y);
    int getDimX(){return this-> dim_x;}
    int getDimY(){return this-> dim_y;}
    int getCreator(unit_t type);
    std::unique_ptr<Selectable>& getElementAt(int x, int y);
    void makeCreator(int ID);
    status_t canPlace(const Position& location, int dim_x, int dim_y);
    bool canDeposit(int x, int y, player_t faction);
    bool canHarvest(int x, int y);
    bool canTraverse(int x, int y);
    bool hasEnemy(int x, int y, player_t player_faction);
    void dealDamage(int x, int y, int damage);
    size_t get_width();
    size_t get_height();
    std::vector<Position> getChangedSandPositions(){return this->changed_sand_positions;}
    void clearSandPositions();
    void addSandPosition(int x, int y);
    std::vector<Position> get_traversable_neighbors_of(Position pos, size_t distance);
    size_t get_distance_between(Position pos1, Position pos2);
    std::vector<Position> getCreatingPositions(unit_t type);
    void addDepositPositions(std::vector<Position> & new_deposit_positions);
    std::vector<Position> & getDepositPositions();
    std::vector<Position> getSurroundings(Position position, int e_dim_x, int e_dim_y);
    void removeCreator(unit_t unit);
    response_t createUnit(player_t faction,unit_t type,int & spice);
    response_t createBuilding(player_t faction,building_t type,int pos_x,int pos_y,int spice,int c_spice,int energy,int c_energy);
    void selectElement(player_t faction,int pos_x, int pos_y);
    void selectElements(player_t faction,int Xmin, int Xmax,int Ymin, int Ymax);
    void reactToPosition(player_t faction, int pos_x, int pos_y);
    int totalElements();
    void sendElements(Protocol & protocol, Socket & client_socket);
    void getTotalChangedCells();
    void clearChangedCells();
}
