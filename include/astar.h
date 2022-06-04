#ifndef ASTAR_H_
#define ASTAR_H_

#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "cell.h"
#include "astar.h"
#include "board.h"

class HashFn {
  public:
    size_t operator() (const Position& p) const {
      return (p.x + p.y) * (p.x + p.y + 1) / 2 + p.x;
    }
};

class aStar {
 public:
    std::vector<Position> adjacents;
    Board& maze;
    std::unordered_map<Position, Position, HashFn> predecessors;
    std::unordered_map<Position, int, HashFn> f;
    std::unordered_map<Position, int, HashFn> g;
    aStar(Board& maze);
    std::vector<Position> algorithm(Position start, Position end);
    std::vector<Position> reconstruct_path(Position current);
    void set_predecessor(Position position, Position predecessor);
    Position get_predecessor_of(Position position);
    Position pop_lowest_f_from(std::vector<Position>& set);
    bool validate_position(Position position);
};


#endif // ASTAR_H_
