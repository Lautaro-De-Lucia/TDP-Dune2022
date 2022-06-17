#pragma once

#include <string>
#include "board.h"
#include "common_utils.h"
#include "client_camera.h"
#include "client_renderhandler.h"

extern Camera cam;

#include <SDL2pp/SDL2pp.hh>

#define TRIKE_PATH "/unitsprites/trike/trike.png"
#define HARVESTER_PATH "/unitsprites/harvester/harvester.png"

#define WIND_TRAP_PATH "/buildingsprites/windtrap/windtrap.png"
#define BARRACK_PATH "/buildingsprites/barrack/barrack.png"
#define REFINERY_PATH "/buildingsprites/refinery/refinery.png"

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
    std::string name;
    int LP;
    int max_LP;
    Position position;
    bool selected;
    health_t health;
 public:
    CSelectable(State& initial_state,SDL2pp::Renderer& renderer, const std::string& lp_path);
    virtual void update(State& new_state,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    virtual void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
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
    std::vector<bool> updates;
 public:
    CMovable(unit_t type,State& state,SDL2pp::Renderer& renderer, const std::string& lp_path , const std::string& path);
    void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    void update(State& new_state,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
}; 

class CStatic : public CSelectable {
 private:
    SDL2pp::Texture texture;  
    building_t type;
 public:
    CStatic(building_t type,State& state,SDL2pp::Renderer& renderer, const std::string& lp_path ,const std::string& path);
    void render(SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
    void update(State& new_state,SDL2pp::Renderer& renderer, int cam_pos_x, int cam_pos_y);
};


//  Client::updateBoard(int toread){
//      int pos_x;
//      int pos_y;
//      int type;
//      for (size_t i = 0; i < toread; i++){    
//          this->protocol.receiveCell(pos_x,pos_y,type);
//          this->map_cells[pos_x][pos_y].setType(type);
//      }
//  }




//  Client::updateElements(int toread){
    //  C: [0,1,2,3,4,5]
    //  U: [0t,1t,2t,3t,4t,5t]
    //  while(i < this->updates.size()){this->updates[i] == false};  
    //  U: [0f,1f,2f,3f,4f,5f]
    //  while(i < toread){
    //  S: [0,1,2,4,5,6,7,8]
    //  this->protocol.receive_selectable_type(type,this->socket)  
    //  switch(type)
    //      case TRIKE: 
    //          this->protocol.receive_trike(id,lp,pos_x,pos_y,selected,attacking);
    //          if(this->contains(id))
    //              this->elements.at(id).update(lp,pos_x,pos_y,selected,attacking);
    //              this->updates[id] == true;
    //          else   
    //              this->elements.add({id,new Trike(TRIKE,lp,pos_x,pos_y,selected,attacking)})
    //              this->updates.push_back(true);
    //           // (ATTR: std::vector<bool> updates)
    //      ...
    //      case REFINERY:
    //          this->protocol.receive_refinery(id,lp,selected);
    //          this->elements.at(id).update(lp,selected);
    //      
    //      }
    //      i++
    //      [0t,1t,2t,3f,4t,5t,6t,7t,8t]
    //  }
    //      //  Destroy elements
    //      for (size_t i = 0 ; i < this->updates.size() ; i++)
    //          if(this->updates[i] == false)
    //              if(this->elements.contains(i))
    //                  this->elements.erase(i);
    //      
    //  void receive_trike(int & id, int & lp, int & pos_x, int & pos_y, bool selected, bool attacking){
    //      this->receiveUnit(id,lp,pos_x,pos_y,selected);      
    //      // lectura del attacking
    //      attacking <- lectura;


//  Client::destroyElements(){
//      std::vector<int> todestroy;
//      for (size_t i = 0 ; i < this->updates.size() ; i++){
//          if(this->contains(i) && this->updates[i] == false)
//              todestroy.push_back(i);
//      }
//      for(int id_to_destroy : this->todestroy)
//        this->elements.erase(id_to_destroy);
//  }
