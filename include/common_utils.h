#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

#define TILE_SIZE 16

//  These would be lifted from a file
#define HARVESTER_LP 600
#define HARVESTER_SPICE_COST 300
#define HARVESTER_SPICE_CAPACITY 50
#define HARVESTER_SPEED 2

#define TRIKE_LP 80
#define TRIKE_SPICE_COST 100
#define TRIKE_SPEED 4
#define TRIKE_ATTACK 2
#define TRIKE_RANGE 4

#define FREMEN_LP 50
#define FREMEN_SPICE_COST 100
#define FREMEN_SPEED 2
#define FREMEN_ATTACK 8
#define FREMEN_RANGE 3

#define INFANTRY_LP 50
#define INFANTRY_SPICE_COST 50
#define INFANTRY_SPEED 2
#define INFANTRY_ATTACK 2
#define INFANTRY_RANGE 3

#define SARDAUKAR_LP 80
#define SARDAUKAR_SPICE_COST 100
#define SARDAUKAR_SPEED 2
#define SARDAUKAR_ATTACK 10
#define SARDAUKAR_RANGE 3

#define TANK_LP 30
#define TANK_SPICE_COST 300
#define TANK_SPEED 3
#define TANK_ATTACK 7
#define TANK_RANGE 3

#define DEVASTATOR_LP 400
#define DEVASTATOR_SPICE_COST 400
#define DEVASTATOR_SPEED 2
#define DEVASTATOR_ATTACK 8
#define DEVASTATOR_RANGE 4

//  These would be lifted from a file
#define CONSTRUCTION_YARD_LP 3000
#define CONSTRUCTION_YARD_ENERGY 2000
#define CONSTRUCTION_YARD_SPICE 0
#define CONSTRUCTION_YARD_DIM_X 3
#define CONSTRUCTION_YARD_DIM_Y 3

#define AIR_TRAP_LP 500
#define AIR_TRAP_ENERGY 500
#define AIR_TRAP_SPICE 800
#define AIR_TRAP_DIM_X 3
#define AIR_TRAP_DIM_Y 3
#define AIR_TRAP_CAPACITY 500

#define BARRACK_LP 300
#define BARRACK_ENERGY -100
#define BARRACK_SPICE 300
#define BARRACK_DIM_X 2
#define BARRACK_DIM_Y 3

#define REFINERY_LP 1000
#define REFINERY_ENERGY -400
#define REFINERY_SPICE 500
#define REFINERY_DIM_X 3
#define REFINERY_DIM_Y 3
#define REFINERY_CAPACITY 5000

#define LIGHT_FACTORY_LP 500
#define LIGHT_FACTORY_ENERGY -500
#define LIGHT_FACTORY_SPICE 800
#define LIGHT_FACTORY_DIM_X 3
#define LIGHT_FACTORY_DIM_Y 3

#define HEAVY_FACTORY_LP 1500
#define HEAVY_FACTORY_ENERGY -800
#define HEAVY_FACTORY_SPICE 1500
#define HEAVY_FACTORY_DIM_X 4
#define HEAVY_FACTORY_DIM_Y 4

#define SILO_LP 200
#define SILO_ENERGY -100
#define SILO_SPICE 200
#define SILO_SPICE_CAPACITY 1000
#define SILO_DIM_X 1
#define SILO_DIM_Y 1

#define PALACE_LP 1000
#define PALACE_ENERGY -800
#define PALACE_SPICE 2000
#define PALACE_DIM_X 3
#define PALACE_DIM_Y 3

#define SAND_SPICE_FULL 1000
#define SAND_SPICE_SOME 500
#define SAND_SPICE_LITTLE 200
#define SAND_SPICE_EMPTY 0

#define RESPONSE_FAILURE_OFFSET 20

#define FONT_IMPACT_PATH "/fonts/impact/impact.ttf"
#define FONT_IMPACTED_PATH "/fonts/impact/Impacted.ttf"

#define IMG_FORMAT ".png"

/*
#define TRIKE_PATH "/unitsprites/trike"
#define HARVESTER_PATH "/unitsprites/harvester"
#define TANK_PATH "/unitsprites/tank"
#define INFANTRY_PATH "/unitsprites/infantry"
#define FREMEN_PATH "/unitsprites/fremen"
#define DEVASTATOR_PATH "/unitsprites/devastator"
#define SARDAUKAR_PATH "/unitsprites/sardaukar"
*/

#define TRIKE_SHOT_PATH "/unitshots/trike/shot"
#define FREMEN_SHOT_PATH "/unitshots/fremen/shot"
#define INFANTRY_SHOT_PATH "/unitshots/infantry/shot"
#define SARDAUKAR_SHOT_PATH "/unitshots/sardaukar/shot"
#define TANK_SHOT_PATH "/unitshots/tank/shot"
#define DEVASTATOR_SHOT_PATH "/unitshots/devastator/shot"

#define EXPLOSION_PATH "/explosions/explosion"


#define ATREIDES_INIT_POS_X 10
#define ATREIDES_INIT_POS_Y 5

#define HARKONNEN_INIT_POS_X 77 
#define HARKONNEN_INIT_POS_Y 42

#define ORDOS_INIT_POS_X 5 
#define ORDOS_INIT_POS_Y 40


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

constexpr std::initializer_list<player_t> factions = {
    HARKONNEN,
    ATREIDES,
    ORDOS
};

const char * stringify(player_t player);

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
    INFANTRY = 4,
    SARDAUKAR = 5,
    TANK = 6,
    DEVASTATOR = 7
};

constexpr std::initializer_list<unit_t> units = {
    HARVESTER,
    TRIKE,
    FREMEN,
    INFANTRY,
    SARDAUKAR,
    TANK,
    DEVASTATOR
};

const char * stringify(unit_t unit);

enum building_t {
    AIR_TRAP = 1, 
    BARRACK = 2,
    REFINERY = 3,
    LIGHT_FACTORY = 4,
    HEAVY_FACTORY = 5,
    SILO = 6,
    PALACE = 7,
    CONSTRUCTION_YARD = 8
};

constexpr std::initializer_list<building_t> buildings = {
    AIR_TRAP,
    BARRACK, 
    REFINERY,
    LIGHT_FACTORY,
    HEAVY_FACTORY,
    SILO,
    PALACE,
    CONSTRUCTION_YARD
};

constexpr std::initializer_list<building_t> creators = {
    BARRACK, 
    LIGHT_FACTORY,
    HEAVY_FACTORY,
};

const char * stringify(building_t building);

enum direction_t {
    TOP = 0,
    TOP_RIGHT = 1,
    RIGHT = 2,
    BOTTOM_RIGHT = 3,
    BOTTOM = 4,
    BOTTOM_LEFT = 5,
    LEFT = 6,
    TOP_LEFT = 7
};

constexpr std::initializer_list<direction_t> directions = {
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    LEFT,
    TOP_LEFT
};

const char * stringify(direction_t direction);

enum frame_t {
    ANIMATION_IDLE = 0,
    MOVING_1 = 1,
    MOVING_2 = 2,
    SPECIAL = 3,
};

constexpr std::initializer_list<frame_t> animations = {
    ANIMATION_IDLE,
    MOVING_1,
    MOVING_2,
    SPECIAL
};

const char * stringify(frame_t animation);


enum selectable_t {
    SEL_HARVESTER = 1,
    SEL_TRIKE = 2,
    SEL_FREMEN = 3,
    SEL_INFANTRY = 4,
    SEL_SARDAUKAR = 5,
    SEL_TANK = 6,
    SEL_DEVASTATOR = 7,
    SEL_AIR_TRAP = 8,
    SEL_BARRACK = 9,
    SEL_REFINERY = 10,
    SEL_LIGHT_FACTORY = 11,
    SEL_HEAVY_FACTORY = 12,
    SEL_SILO = 13,
    SEL_PALACE = 14,
    SEL_CONSTRUCTION_YARD = 15
};

struct instruction_t {
    int player_ID;
    player_t faction;
    command_t command;

    instruction_t(int player_ID, player_t faction, command_t command) {
        this->player_ID = player_ID;
        this->faction = faction;
        this->command = command;
    }

    virtual ~instruction_t(){}

};

struct creation_t {
    int creator_ID;
	unit_t unit_being_created;
	int current_time;
	int total_time;
    creation_t(int creator_ID, unit_t unit, int current_time, int total_time) {
        this->creator_ID = creator_ID;
        this->unit_being_created = unit;
        this->current_time = current_time;
        this->total_time = total_time;
    }
};

struct building_create_t : instruction_t {

    int type;
    int pos_x;
    int pos_y;

    building_create_t(int player_ID, player_t faction, int type, int pos_x, int pos_y) : instruction_t(player_ID, faction, CREATE_BUILDING) {
        this->type = type;
        this->pos_x = pos_x;
        this->pos_y = pos_y;
    }
};

struct unit_create_t : instruction_t {

    unit_t type;

    unit_create_t(int player_ID, player_t faction, unit_t type) : instruction_t(player_ID, faction, CREATE_UNIT) {
        this->type = type;
    }

};

struct left_click_t : instruction_t {

    int pos_x;
    int pos_y;

    left_click_t(int player_ID, player_t faction, int pos_x, int pos_y) : instruction_t(player_ID, faction, MOUSE_LEFT_CLICK) {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
    }
};

struct right_click_t : instruction_t {

    int pos_x;
    int pos_y;

    right_click_t(int player_ID, player_t faction, int pos_x, int pos_y) : instruction_t(player_ID, faction, MOUSE_RIGHT_CLICK) {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
    }
};

struct selection_t : instruction_t {

    int Xm;
    int XM;
    int Ym;
    int YM;

    selection_t(int player_ID, player_t faction, int Xm, int XM, int Ym, int YM) : instruction_t(player_ID, faction, MOUSE_SELECTION) {
        this->Xm = Xm;
        this->XM = XM;
        this->Ym = Ym;
        this->YM = YM;
    }
};

struct idle_t : instruction_t {

    idle_t(int player_ID, player_t faction) : instruction_t(player_ID, faction, IDLE) {
    }
};

struct close_t : instruction_t {

    close_t(int player_ID, player_t faction) : instruction_t(player_ID, faction, CLOSE) {
    }
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
Position pixelToGP(int x, int y);
void GPToPixel(Position pos, int& pixel_x, int& pixel_y);

enum hud_button_t {
    UNKNOWN_BTN = 0,
    BUILD_BTN = 1,
    UNIT_BTN = 2
};



// falta agregar una mayor diversidad de errores
enum response_t {

    RES_SUCCESS = RESPONSE_FAILURE_OFFSET + 0,
    RES_CREATE_UNIT_SUCCESS = RESPONSE_FAILURE_OFFSET+ 1,
    RES_CREATE_BUILDING_SUCCESS = RESPONSE_FAILURE_OFFSET + 5,

    RES_CREATE_UNIT_FAILURE_SPICE = RESPONSE_FAILURE_OFFSET - 2,
    RES_CREATE_UNIT_FAILURE_CREATOR = RESPONSE_FAILURE_OFFSET - 3,
    RES_CREATE_UNIT_FAILURE_SPACE = RESPONSE_FAILURE_OFFSET - 4,
    RES_CREATE_UNIT_FAILURE_SPECIAL = RESPONSE_FAILURE_OFFSET - 5,
    RES_WAIT_TO_BUILD = RESPONSE_FAILURE_OFFSET -9,

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

struct rgb_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    rgb_t(){
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
    void set (uint8_t r,uint8_t g,uint8_t b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};


constexpr std::initializer_list<const char *> maptiles = {
    "d0","d1","d2","d3","d4",
    "p0","p1","p2","p3","p4","p5","p6","p7","p8",
    "r0","r1","r2","r3","r4","r5","r6","r7","r8",
    "s0","s1","s2","s3"
};

// ===============================================
// ===============================================
