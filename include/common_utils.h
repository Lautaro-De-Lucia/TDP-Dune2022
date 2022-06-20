#pragma once

#include <string>
#include <iostream>
#include <map>
#include <SDL2pp/SDL2pp.hh>

#define TILE_SIZE 16

//  These would be lifted from a file
#define HARVESTER_LP 500
#define HARVESTER_SPICE_COST 500
#define HARVESTER_SPICE_CAPACITY 50
#define HARVESTER_SPEED 2
#define HARVESTER_DIM_X 1
#define HARVESTER_DIM_Y 1

#define TRIKE_LP 800
#define TRIKE_SPICE_COST 500
#define TRIKE_SPEED 5
#define TRIKE_ATTACK 3
#define TRIKE_RANGE 4
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

#define LIGHT_FACTORY_LP 500
#define LIGHT_FACTORY_ENERGY 500
#define LIGHT_FACTORY_SPICE 800
#define LIGHT_FACTORY_DIM_X 3
#define LIGHT_FACTORY_DIM_Y 3

#define HEAVY_FACTORY_LP 1500
#define HEAVY_FACTORY_ENERGY 800
#define HEAVY_FACTORY_SPICE 1500
#define HEAVY_FACTORY_DIM_X 4
#define HEAVY_FACTORY_DIM_Y 4

#define SILO_LP 200
#define SILO_ENERGY 100
#define SILO_SPICE 200
#define SILO_SPICE_CAPACITY 1000
#define SILO_DIM_X 1
#define SILO_DIM_Y 1

#define PALACE_LP 1000
#define PALACE_ENERGY 800
#define PALACE_SPICE 2000
#define PALACE_DIM_X 3
#define PALACE_DIM_Y 3

#define SAND_SPICE_FULL 1000
#define SAND_SPICE_SOME 500
#define SAND_SPICE_LITTLE 200
#define SAND_SPICE_EMPTY 0

#define RESPONSE_FAILURE_OFFSET 20

#define FONT_IMPACT_PATH "/fonts/impact/impact.ttf"

enum color_t {
    RED,
    BLUE,
    GREEN,
    YELLOW
};

enum player_t {
    HARKONNEN = 1,
    ATREIDES = 2,
    ORDOS = 3
};

enum command_t {
    CLOSE = 0,
    CREATE_BUILDING = 1,
    CREATE_UNIT = 2, 
    MAKE_CREATOR = 3,
    MOUSE_LEFT_CLICK = 4,
    MOUSE_RIGHT_CLICK = 5,
    MOUSE_SELECTION = 6,
    TEST_ASTAR = 7,
    IDLE = 8,
};

enum cell_t {
    ROCK = 0,
    SAND = 1,
    DUNE = 2,
    PIT = 3,
    SAND_EMPTY = 4,
    SAND_LITTLE = 5,
    SAND_SOME = 6,
    SAND_FULL = 7,
    DEBRIS = 8
};

enum unit_t {
    HARVESTER = 1,
    TRIKE = 2,
    FREMEN = 3,
    LIGHT_INFANTRY = 4,
    SARDAUKAR = 5,
    TANK = 6,
    DEVASTATOR = 7
};

enum building_t {
    AIR_TRAP = 1, 
    BARRACK = 2,
    REFINERY = 3,
    LIGHT_FACTORY = 4,
    HEAVY_FACTORY = 5,
    SILO = 6,
    PALACE = 7
};

enum selectable_t {
    SEL_HARVESTER = 1,
    SEL_TRIKE = 2,
    SEL_AIR_TRAP = 3,
    SEL_BARRACK = 4,
    SEL_REFINERY = 5,
    SEL_LIGHT_FACTORY = 6,
    SEL_HEAVY_FACTORY = 7,
    SEL_SILO = 8,
    SEL_PALACE = 9
};

struct Position {
 public:
    int x;
    int y;

    Position();
    Position(int x, int y);
    Position(const Position& o);
    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
    bool operator==(const Position& o) const { return o.x == x && o.y == y; };
    Position operator+(const Position& o) { return Position( o.x + x, o.y + y ); };
    Position& operator=(const Position& o) { if (this == &o) return *this; this->x = o.x; this->y = o.y; return *this; };
    void print();
};

struct State {
    int ID;
    int LP;
    Position position;
    bool selected;
    bool special;
};

struct Area {
    int Xmin;
    int Xmax;
    int Ymin;
    int Ymax;
    Area(int Xmin,int Xmax, int Ymin, int Ymax){
        this->Xmin=Xmin;
        this->Xmax=Xmax;
        this->Ymin=Ymin;
        this->Ymax=Ymax;
    }
};

void sleepcp(int milliseconds);

enum hud_button_t {
    UNKNOWN_BTN = 0,
    BUILD_BTN = 1,
    UNIT_BTN = 2
};



// falta agregar una mayor diversidad de errores
enum response_t {

    RES_SUCCESS = RESPONSE_FAILURE_OFFSET + 0,

    RES_CREATE_UNIT_SUCCESS = RESPONSE_FAILURE_OFFSET+ 1,
    RES_CREATE_UNIT_FAILURE_SPICE = RESPONSE_FAILURE_OFFSET - 2,
    RES_CREATE_UNIT_FAILURE_CREATOR = RESPONSE_FAILURE_OFFSET - 3,
    RES_CREATE_UNIT_FAILURE_SPACE = RESPONSE_FAILURE_OFFSET - 4,

    RES_CREATE_BUILDING_SUCCESS = RESPONSE_FAILURE_OFFSET + 5,
    RES_CREATE_BUILDING_FAILURE_SPICE = RESPONSE_FAILURE_OFFSET - 6,
    RES_CREATE_BUILDING_FAILURE_ENERGY = RESPONSE_FAILURE_OFFSET - 7,
    RES_CREATE_BUILDING_FAILURE_TERRAIN = RESPONSE_FAILURE_OFFSET - 8,

    RES_SELECTED_BUILDING_IS_NOW_CREATOR = RESPONSE_FAILURE_OFFSET + 9
};

// ================================================
// ================================================
// ================== EXCEPTIONS ==================
// ================================================
// ================================================


class game_already_exists_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "game_already_exists\n";
    }
};

class game_not_found_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "game_not_found\n";
    }
};

class game_is_full_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "game_is_full\n";
    }
};

class invalid_invocation_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "invalid_invocation\n";
    }
};

class invalid_house_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "invalid_house\n";
    }
};

class invalid_command_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "invalid_command\n";
    }
};

class was_closed_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "was_closed\n";
    }
};

class failure_to_send_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "failure_to_send\n";
    }
};

class failure_to_receive_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "failure_to_receive\n";
    }
};

class failure_to_create_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "failure_to_create\n";
    }
};

class failure_to_join_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "failure_to_join\n";
    }
};

class getaddrinfo_failed_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "getaddrinfo_failed\n";
    }
};

class socket_connection_failed_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "socket_connection_failed\n";
    }
};

class accept_failed_exception : public std::exception {
 public:
    const char * what() const throw() {
        return "accept_failed\n";
    }
};

// ===============================================
// ===============================================
