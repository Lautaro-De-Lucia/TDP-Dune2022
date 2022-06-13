#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "common_utils.h"

class Selectable {
 protected:
    player_t faction;
    std::string name;
    std::string imagepath;
    int LP;
    Position position;
    int dim_x;
    int dim_y;
    bool selected;
    bool is_movable;
    std::vector<Position> remaining_path;

 public:
    Selectable(player_t faction, int LP, Position position, int dim_x, int dim_y, bool moves);
    void selection(Area& mouse_area);
    bool isSelected();
    bool moves();
    void select();
    void unselect();
    bool contains(const Position& pos);
    bool isWithin(const Area& selection);
    virtual void react(int x, int y, Board& board);
    virtual void update(State & state, Board& board);
    player_t getFaction();
    std::string getName();
    std::vector<Position> getPositions();
    std::vector<Position> getSurroundings(); 
    void setPosition(Position position);
    Position getPosition();
    int getDimX();
    int getDimY();
    void getState(State& state);
    std::vector<Position>& get_remaining_path();
    virtual ~Selectable(){}
};
