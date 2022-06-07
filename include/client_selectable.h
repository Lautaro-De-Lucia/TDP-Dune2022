#pragma once

#include <string>
#include "board.h"
#include "common_utils.h"
#include "client_camera.h"

extern Camera cam;

#include <SDL2pp/SDL2pp.hh>

#define TRIKE_PATH "/unitsprites/trike/trike.png"
#define HARVESTER_PATH "/unitsprites/harvester/harvester.png"

#define WIND_TRAP_PATH "/buildingsprites/windtrap/windtrap.png"
#define BARRACK_PATH "/buildingsprites/barrack/barrack.png"
#define REFINERY_PATH "/buildingsprites/refinery/refinery.png"

#define LP_PATH "/extrasprites/lp.png"

enum move_direction {
    TOP = 0,
    TOP_RIGHT = 1,
    RIGHT = 2,
    BOTTOM_RIGHT = 3,
    BOTTOM = 4,
    BOTTOM_LEFT = 5,
    LEFT = 6,
    TOP_LEFT = 7
};

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
    uint16_t ID;
    std::string name;
    uint16_t LP;
    uint16_t max_LP;
    Position position;
    bool selected;
    health_t health;
public:
    CSelectable(std::string name,State & initial_state,SDL2pp::Renderer & renderer, const std::string& lp_path);
    virtual void update(State & new_state,SDL2pp::Renderer & renderer);
    virtual void render(SDL2pp::Renderer & renderer);
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
  public:
      CMovable(std::string name,State & state,SDL2pp::Renderer & renderer, const std::string& lp_path , const std::string& path);
      void render(SDL2pp::Renderer & renderer);
      void update(State & new_state,SDL2pp::Renderer & renderer);
}; 

class CStatic : public CSelectable {
  private:
      SDL2pp::Texture texture;  
  public:
      CStatic(std::string name,State & state,SDL2pp::Renderer & renderer, const std::string& lp_path ,const std::string& path);
      void render(SDL2pp::Renderer & renderer);
      void update(State & new_state,SDL2pp::Renderer & renderer);
};