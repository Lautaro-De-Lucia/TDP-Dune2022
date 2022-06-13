#pragma once 

#include "selectable.h"
#include "common_utils.h"

#include "astar.h"

class Unit : public Selectable {
 protected:
    std::string name;
    bool moving;
    int spice;
    int speed;
    size_t current_time;
    size_t movement_time;

 public:
    Unit() = default;
    Unit(player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed);
    bool place(Board& board, std::vector<Position>& positions, int& spice);
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    void move(int x, int y, Board& board);

};


class Harvester : public Unit {
    int stored_spice;
    int max_spice;
    bool harvesting;
    Position harvest_position;
 public:
    Harvester(player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int max_spice);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    void harvest(int x, int y, Board& board);
};


class Trike : public Unit {
    std::string name;
    int spice;
    int speed;
    int attack;
 public:
    Trike(player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
};
