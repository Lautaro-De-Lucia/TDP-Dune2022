#pragma once

#include <string>
#include <iostream>

#define TILE_SIZE 16

//  These would be lifted from a file
#define HARVESTER_LP 500
#define HARVESTER_SPICE_COST 500
#define HARVESTER_SPICE_CAPACITY 800
#define HARVESTER_SPEED 2
#define HARVESTER_DIM_X 1
#define HARVESTER_DIM_Y 1

#define TRIKE_LP 800
#define TRIKE_SPICE_COST 500
#define TRIKE_SPEED 5
#define TRIKE_ATTACK 3
#define TRIKE_DIM_X 1
#define TRIKE_DIM_Y 1

//  These would be lifted from a file
#define AIR_TRAP_LP 500
#define AIR_TRAP_ENERGY 500
#define AIR_TRAP_SPICE 800
#define AIR_TRAP_DIM_X 3
#define AIR_TRAP_DIM_Y 3
#define AIR_TRAP_CAPACITY 500

#define BARRACK_LP 300
#define BARRACK_ENERGY 100
#define BARRACK_SPICE 300
#define BARRACK_DIM_X 2
#define BARRACK_DIM_Y 3

#define REFINERY_LP 1000
#define REFINERY_ENERGY 400
#define REFINERY_SPICE 500
#define REFINERY_DIM_X 3
#define REFINERY_DIM_Y 3
#define REFINERY_CAPACITY 5000

#define DEF_SAND_SPICE 200

enum player_t {
    HARKONNEN = 1,
    ATREIDES = 2,
    ORDOS = 3
};

enum unit_t{
    HARVESTER = 1,
    TRIKE = 2
}; 

enum cell_t{
    ROCK = 0,
    SAND = 1,
    DUNE = 2,
    PIT = 3
};

enum building_t {
    AIR_TRAP = 1, 
    BARRACK = 2,
    REFINERY = 3
};

struct Position {
 public:
    int x;
    int y;

    Position();
    Position(int x, int y);
    Position(const Position& o);
    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
    bool operator==( const Position& o ) const { return o.x == x && o.y == y; };
    Position operator+( const Position& o ) { return Position( o.x + x, o.y + y ); };
    Position& operator=(const Position& o) { if (this == &o) return *this; this->x = o.x; this->y = o.y; return *this; };
    void print();
};

struct State {
    int ID;
    int LP;
    Position position;
    bool selected;
};

struct Area {
    int Xmin;
    int Xmax;
    int Ymin;
    int Ymax;
    Area(int Xmin,int Xmax, int Ymin, int Ymax);
};

void sleepcp(int milliseconds);
