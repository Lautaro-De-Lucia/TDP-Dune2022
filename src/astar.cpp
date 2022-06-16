#include <vector>
#include <algorithm>
#include <iostream>

#include "astar.h"

#define INF 999999

aStar::aStar() {

    (this->adjacents).push_back(Position( 0, -1));
    (this->adjacents).push_back(Position( 0,  1));
    (this->adjacents).push_back(Position(-1,  0));
    (this->adjacents).push_back(Position( 1,  0));
    (this->adjacents).push_back(Position(-1, -1));
    (this->adjacents).push_back(Position(-1,  1));
    (this->adjacents).push_back(Position( 1, -1));
    (this->adjacents).push_back(Position( 1,  1));
}

void aStar::set_predecessor(Position position, Position predecessor) {
    (this->predecessors)[position] = predecessor;
}

Position aStar::get_predecessor_of(Position position) {
    return (this->predecessors)[position];
}

Position aStar::pop_lowest_f_from(std::vector<Position>& set) {

    size_t lowest_f_position_index = 0;
    Position lowest_f_position(set[lowest_f_position_index]);
    for (size_t i = 0; i < set.size(); i++) {
        
        if ((this->f)[set[i]] < (this->f)[lowest_f_position]) {
            lowest_f_position_index = i;
            lowest_f_position = set[lowest_f_position_index];
        }
    }
    // pop
    set.erase(set.begin() + lowest_f_position_index);
    return lowest_f_position;    
}

std::vector<Position> aStar::reconstruct_path(Position current) {
    std::vector<Position> path;
    Position none_position(-1, -1);
    while (1) {
        path.push_back(current);
        if (get_predecessor_of(current) == none_position)
            break;
        current = (this->predecessors)[(current)];
    }

    return path;
}

bool aStar::validate_position(Position position,Board& board) {
    // position.x out of bounds
    if (position.x > board.get_width() - 1 || position.x < 0) 
        return false;
    // position.y out of bounds
    if (position.y > board.get_height() - 1 || position.y < 0) 
        return false;
    // position is an obstacle
    if (board.getCell(position.x, position.y).canTraverse() == false)
        return false;
    return true;
}

std::vector<Position> aStar::algorithm(Position start, Position end, Board& board) {

    std::vector<Position> empty_path;

    if (start == end)
        return empty_path;

    size_t dist_from_start_to_end = board.get_distance_between(start, end);
    
    if((dist_from_start_to_end == 1)) {

        std::vector<Position> simple_path;

        if (validate_position(end,board)) {
            simple_path.push_back(end);
            simple_path.push_back(start);
            return simple_path;
        }
        return empty_path;        
    }

    size_t board_width = board.get_width();
    size_t board_height = board.get_height();

    for (size_t i = 0; i < board_width; i++) {
        for (size_t j = 0; j < board_height; j++) {
            Position position(i, j);
            (this->f)[position] = INF;
            (this->g)[position] = INF;
        }
    }

    if (!validate_position(end,board)) {      

        size_t dist_from_end_to_new_end = 1;
        std::vector<Position> neighbors;

        size_t i = 1;

        while (neighbors.size() == 0) {

            if (dist_from_end_to_new_end >= dist_from_start_to_end)
                return empty_path;
            neighbors = board.get_traversable_neighbors_of(end, dist_from_end_to_new_end);
            dist_from_end_to_new_end++;
        }  

        Position closest_neighbor = neighbors.front();
        size_t closest_distance = board.get_distance_between(start, closest_neighbor);
        for (Position neighbor : neighbors) {
            size_t new_dist = board.get_distance_between(start, neighbor);
            if (new_dist < closest_distance) {
                closest_neighbor = neighbor;
                closest_distance = new_dist;
            }
        }
        end = closest_neighbor;
    }

    

    Position none_position(-1, -1);
    set_predecessor(start, none_position);
    set_predecessor(end, none_position);

    std::vector<Position> open_set;
    open_set.push_back(start);
    (this->g)[start] = 0;
    (this->f)[start] = 0;

    while (!open_set.empty()) {

        Position current(pop_lowest_f_from(open_set));

        if (current == end)
            return reconstruct_path(current);

        // getting neighbors of current
        std::vector<Position> neighbors;
        for (Position offset : this->adjacents) {
            int pos_x = (current).x + offset.x;
            int pos_y = (current).y + offset.y;

            if (!validate_position(Position(pos_x, pos_y),board))
                continue;
            
            Position new_position(pos_x, pos_y);
            neighbors.push_back(new_position);
        }

        for (Position neighbor : neighbors) {

            int neighbor_g = (this->g)[current] + 1;

            if (neighbor_g < (this->g)[neighbor])  {

                //int neighbor_h = ((neighbor.x - end.x)*(neighbor.x - end.x)) + ((neighbor.y - end.y)*(neighbor.y - end.y));
                int neighbor_h = abs(neighbor.x - end.x) + abs(neighbor.y - end.y);
                // evaluar costos forzando mejor camino

                int neighbor_f = neighbor_g + neighbor_h;

                (this->predecessors)[neighbor] = current;
                (this->g)[neighbor] = neighbor_g;
                (this->f)[neighbor] = neighbor_f;

                bool was_not_in_set = true;

                for (Position position : open_set)
                    if (position == neighbor)
                        was_not_in_set = false;

                if (was_not_in_set)
                    open_set.push_back(neighbor);
            }               
        }
    }
    return empty_path;
}
