#pragma once

#include <queue>

#include "server_selectable.h"
#include "server_astar.h"

#include "common_utils.h"

class Unit : public Selectable {
 protected:
    std::string name;
    direction_t direction;
    bool moving;
    bool targeting;
    int spice;
    int speed;
    size_t current_time;
    size_t movement_time;
    std::queue<Position> pending_move;

 public:
    Unit() = default;
    Unit(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed);
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
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
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
    int getSpice();
    bool isHarvesting();

};


class Trike : public Unit {
    std::string name;
    int speed;
    int attack_points;
    int range;
    bool attacking;
    Position enemy_position;
    Position moving_position;
    void attack(int x, int y, Board & board);
    void occupy(Board & board);
    bool enemySearch(Board & board);
 public:
    Trike(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack, int range);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
    bool isAttacking();
};

class Fremen : public Unit {
    std::string name;
    int speed;
    int attack_points;
    int range;
    bool attacking;
    Position enemy_position;
    Position moving_position;
    void attack(int x, int y, Board & board);
    void occupy(Board & board);
    bool enemySearch(Board & board);
 public:
    Fremen(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack, int range);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
    bool isAttacking();
};

class Infantry : public Unit {
    std::string name;
    int speed;
    int attack_points;
    int range;
    bool attacking;
    Position enemy_position;
    Position moving_position;
    void attack(int x, int y, Board & board);
    void occupy(Board & board);
    bool enemySearch(Board & board);
 public:
    Infantry(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack, int range);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
    bool isAttacking();
};

class Sardaukar : public Unit {
    std::string name;
    int speed;
    int attack_points;
    int range;
    bool attacking;
    Position enemy_position;
    Position moving_position;
    void attack(int x, int y, Board & board);
    void occupy(Board & board);
    bool enemySearch(Board & board);
 public:
    Sardaukar(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack, int range);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
    bool isAttacking();
};

class Tank : public Unit {
    std::string name;
    int speed;
    int attack_points;
    int range;
    bool attacking;
    Position enemy_position;
    Position moving_position;
    void attack(int x, int y, Board & board);
    void occupy(Board & board);
    bool enemySearch(Board & board);
 public:
    Tank(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack, int range);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
    bool isAttacking();
};

class Devastator : public Unit {
    std::string name;
    int speed;
    int attack_points;
    int range;
    bool attacking;
    Position enemy_position;
    Position moving_position;
    void attack(int x, int y, Board & board);
    void occupy(Board & board);
    bool enemySearch(Board & board);
 public:
    Devastator(int ID,player_t faction, int LP, int spice, Position pos, int dim_x, int dim_y, int speed, int attack, int range);
    void create();
    virtual void react(int x, int y, Board& board);
    virtual void update(State& state, Board& board);
    virtual void receiveDamage(int damage);
    virtual void sendState(Protocol & protocol, Socket & client_socket);
    virtual response_t place(Board& board,std::vector<Position>& positions,int* spice);
    bool isAttacking();
};