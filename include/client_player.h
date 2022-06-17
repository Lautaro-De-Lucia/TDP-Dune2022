#pragma once

#include <vector>
#include <map>
#include <unistd.h>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "common_utils.h"
#include "client_selectable.h"
#include "client_gamemap.h"
#include "client_gamehud.h"
#include "client_printer.h"
#include "protocol.h"
#include "mouse.h"
#include "protocol.h"


class CPlayer {
 private:
    size_t spice;
    size_t c_spice;
    size_t c_energy;
    int energy;
    double efficiency;
    std::unique_ptr<SDL2pp::Texture> user_message;
    std::map<int, std::unique_ptr<CSelectable>> elements;
    MouseHandler mouse;
    CPrinter printer;
    GameMap map;
    GameHud hud;
    SDL2pp::Window& game_window;
    SDL2pp::Renderer& game_renderer;
    Camera& camera;
    bool is_holding_building;
    int building_held;


 public:
    CPlayer(Camera& cam, SDL2pp::Window& window,SDL2pp::Renderer& renderer, size_t spice, size_t c_spice, int energy, size_t c_energy, std::vector<std::vector<std::string>>& map_data);
    void renderMap();
    void renderHud();
    void addElement(unit_t type,State& desc);
    void addElement(building_t type,State& desc);
    void update(std::vector<State>& server_data, int spice, int energy);
    void clientUpdate(std::vector<int>& mouse_event);
    //   Socket/Protocol methods
    void updateElements(int toread);
    void updateBoard(int toread);

    void print(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time);
    
    void addUnitButton(std::string& IMG_PATH, int& x, int& y, int& id);
    void addBuildButton(std::string& IMG_PATH, int& x, int& y, int& id);
    int checkBuild(int& x, int& y);
    int checkUnit(int& x, int& );
    bool checkHud(int& x, int& y);
    hud_button_t checkBtn(int& x, int& y);
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