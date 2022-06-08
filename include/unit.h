#pragma once 

#include "selectable.h"
#include "common_utils.h"

#include "astar.h"

/*
class Unit : public Selectable {
 protected:
    std::string name;
    int spice;
    int speed;

 public:
    Unit() = default;
    Unit(int LP,int spice, Position pos, int dim_x, int dim_y,int speed);
    bool place(Board & board,std::vector<Position> & positions,int & spice);
    virtual void react(Cell & location);
    virtual ~Unit();
};
*/


/*
class Harvester : public Unit {
   int stored_spice;
   int max_spice;
public:
    Harvester(int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int stored_spice, int max_spice);
    void create();
    void react(Cell & location);
};
*/
class Trike : public Selectable {
   std::string name;
   int spice;
   int speed;
   int attack;
public:
    Trike(int LP,int spice, Position pos, int dim_x, int dim_y,int speed,int attack);
    void create();
    void react(Cell & location);
    bool place(Board & board,std::vector<Position> & positions,int & spice);
    ~Trike();
};
