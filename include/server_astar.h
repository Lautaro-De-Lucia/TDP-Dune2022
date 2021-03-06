#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "server_cell.h"
#include "server_astar.h"
#include "server_board.h"

#define INF 999999

class HashFn {
 public:
    size_t operator() (const Position& p) const {
      return (p.x + p.y) * (p.x + p.y + 1) / 2 + p.x;
    }
};

class aStar {
 public:
    std::vector<Position> adjacents;
    std::unordered_map<Position, Position, HashFn> predecessors;
    std::unordered_map<Position, int, HashFn> f;
    std::unordered_map<Position, int, HashFn> g;
    aStar();
    std::vector<Position> algorithm(Position start, Position end, Board& board);
    std::vector<Position> reconstruct_path(Position current);
    void set_predecessor(Position position, Position predecessor);
    Position get_predecessor_of(Position position);
    Position pop_lowest_f_from(std::vector<Position>& set);
    bool validate_position(Position position, Board& board);
};
