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
    Unit(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed);
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual bool place(Board& board,std::vector<Position>& positions,int* spice);
    void move(int x, int y, Board& board);

};


class Harvester : public Unit {
    int stored_spice;
    int max_spice;
    int * player_spice;
    bool harvesting;
    bool depositing;
    Position harvest_position;
    Position deposit_position;
    void harvest(int x, int y, Board& board);
    void deposit(Board & board);
    void deposit(int x, int y,Board & board);
    void occupy(Board & board);
 public:
    Harvester(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int max_spice);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual bool place(Board& board,std::vector<Position>& positions,int* spice);

};


class Trike : public Unit {
    std::string name;
    int speed;
    int attack_points;
    int range;
    bool attacking;
    Position enemy_position;
    void attack(int x, int y, Board & board);
    void occupy(Board & board);
    bool enemySearch(Board & board);
 public:
    Trike(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack, int range);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual bool place(Board& board,std::vector<Position>& positions,int* spice);
};
