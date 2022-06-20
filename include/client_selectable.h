#pragma once

#include <string>

#include "client_camera.h"
#include "client_renderhandler.h"

#include "common_utils.h"

extern Camera cam;

#include <SDL2pp/SDL2pp.hh>

#define TRIKE_PATH "/unitsprites/trike/trike.png"
#define HARVESTER_PATH "/unitsprites/harvester/harvester.png"

#define WIND_TRAP_PATH "/buildingsprites/windtrap/windtrap.png"
#define BARRACK_PATH "/buildingsprites/barrack/barrack.png"
#define REFINERY_PATH "/buildingsprites/refinery/refinery.png"
#define LIGHT_FACTORY_PATH "/buildingsprites/lightfactory/lightfactory.png"
#define HEAVY_FACTORY_PATH "/buildingsprites/heavyfactory/heavyfactory.png"
#define SILO_PATH "/buildingsprites/silo/silo.png"
#define PALACE_PATH "/buildingsprites/palace/palace.png"


#define LP_PATH "/extrasprites/lp.png"

#define HEALTH_SPRITES 7

enum health_t {
    VERY_HIGH = 1,
    HIGH = 2,
    MEDIUM_HIGH = 3,
    MEDIUM = 4,
    MEDIUM_LOW = 5,
    LOW = 6,
    VERY_LOW = 7
};

class CSelectable {
 protected:
    SDL2pp::Texture lp_texture;  
    RenderHandler render_handler;
    int ID;
    int faction;
    std::string name;
    int LP;
    int max_LP;
    Position position;
    bool selected;
    health_t health;
 public:
    CSelectable(int id,int faction,int lp,int pos_x,int pos_y,SDL2pp::Renderer& renderer, const std::string& lp_path);
    virtual void update(player_t player_faction, int lp,int pos_x,int pos_y,bool selected,bool special,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    virtual void render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    int get_life_points();
    int getID();
};

//  Tengo que definir una clase CMovable y otra clase CStatic
//  Esto por que la primera tiene que trabajar con una sprite y la otra con una textura sola
//  Por supuesto, CStatic va a ser la más fácil de renderizar asi q arranco con esa
//  El método update también va a diferir por que al haber un cambio de posición debe pasarse 
//  a otro corte de la sprite en el caso de la unidad

class CMovable : public CSelectable {
 private:
    SDL2pp::Texture texture;  
    move_direction dir;
    unit_t type;
    bool special;
 public:
    CMovable(unit_t type,int id,int faction,int lp,int pos_x,int pos_y,SDL2pp::Renderer& renderer, const std::string& lp_path , const std::string& path);
    void render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    void update(player_t player_faction, int lp,int pos_x,int pos_y,bool selected,bool special,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
}; 

class CStatic : public CSelectable {
 private:
    SDL2pp::Texture texture;  
    building_t type;
 public:
    CStatic(building_t type,int id,int faction,int lp,int pos_x,int pos_y,SDL2pp::Renderer& renderer, const std::string& lp_path ,const std::string& path);
    void render(player_t player_faction, SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    void update(player_t player_faction, int lp,bool selected,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
};

