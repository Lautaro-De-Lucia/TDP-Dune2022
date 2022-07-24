#pragma once

#include <queue>
#include <iostream>

#include "client_selectable.h"
#include "client_gamemap.h"
#include "client_gamehud.h"
#include "client_printer.h"
#include "client_mouse.h"
#include "client_camera.h"
#include "client_audioplayer.h"
#include "client_shotshandler.h"
#include "client_texturehandler.h"
#include "client_explosionshandler.h"

#include "common_utils.h"
#include "common_protocol.h"
#include "common_socket.h"

struct corpse_t {
  selectable_t type;
  Position position;
  clock_t time;
  corpse_t(selectable_t type, Position position, int time) {
      this->type = type;
      this->position = position;
      this->time = time;
  }
};

class Player {

 private:
   Protocol protocol;
   Socket socket;
 public:
   size_t spice;
   size_t c_spice;
   size_t c_energy;
   double construction_time;
   int energy;
   int time_penalty;
   std::vector<creation_t> creation_data;
   std::map<building_t, int> creators;
   std::map<int, std::unique_ptr<CSelectable>> elements;
   MouseHandler mouse;
   CPrinter response_printer;
   CPrinter energy_printer;
   GameMap map;
   GameHud hud;
   SDL2pp::Window& game_window;
   SDL2pp::Renderer& game_renderer;
   TextureHandler & textures;
   Camera & camera;
   bool is_holding_building;
   int building_held;
   std::queue<std::vector<int>> usr_events;
   std::map<int,std::string> button_info_paths;
   std::vector<bool> updates;
   bool left_click;
   bool right_click;
   bool selection;
   bool w_key;
   bool a_key;
   bool s_key;
   bool d_key;   
   bool new_unit_available;
   player_t faction;
   AudioPlayer audio;
   bool game_has_started;
   bool can_update;

   std::map<player_t,color_t> faction_colours ={
     {ATREIDES,BLUE},
     {HARKONNEN,RED},
     {ORDOS,GREEN}
  };

   ShotsHandler shotsHandler;
   ExplosionsHandler explosionsHandler;
   std::vector<corpse_t> corpses;

   clock_t base_alert_delay;

   Position base_position;

   game_status_t game_status;

   int enemies;
   int destroyed_enemies;

   clock_t base_time_instruction;
   clock_t base_time_update;

   Player(player_t faction, const char* host_name, const char* service_name,Camera & cam, SDL2pp::Window& window,SDL2pp::Renderer& renderer,TextureHandler & textures, size_t spice, size_t c_spice, int energy, size_t c_energy, std::vector<std::vector<std::string>>& map_data);

   void play();
   void renderWaitingText();
   void renderMap();
   void renderHud();
   void renderButtonInfo();
   void renderHeldBuilding();
   void addElement(unit_t type,State& desc);
   void addElement(building_t type,State& desc);
   void render();
   void update();
   bool contains(int ID);

   //   Socket/Protocol methods

   void printResponse(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time);
   void printEnergy(std::string toprint, std::string fontpath, int x, int y, int size ,SDL_Color color,size_t time);

   void addUnitButton(std::string& IMG_PATH, int& x, int& y, int& id);
   void addBuildButton(std::string& IMG_PATH, int& x, int& y, int& id);
   int checkBuild(int& x, int& y);
   int checkUnit(int& x, int& );
   bool checkHud(int& x, int& y);
   hud_button_t checkBtn(int& x, int& y);

   void receiveCreationData();
   void renderCreationData();
   void renderPrinters();

   void receiveCreators();
   void renderCreators();

   void removeAttacker(int id);
   void updateAttacker(int id, unit_t type, Position attacker_pos, Position target_pos);

   void addExplosion(int id, selectable_t type, Position pos);

   void addCorpse(selectable_t type, Position position, int time);

   void renderCorpses();

   void triggerBaseAttackAlert(clock_t& current_time);

   bool positionBelongsToBase(Position position);

   void triggerNewUnitAlert(unit_t type);

   void renderGameEnding();

   void updateCamera();

   void handleEvent(SDL_Event& event);

   void sendInstruction(command_t command, std::vector<int>& usr_event);

   void handleResponse();

   void updateLoop();
   void instructionLoop();

   void updateUtils();
   void updateHud();
   void updatePlayerState();
   void updateBoard();
   void updateSelectables();
   void updateCreationData();
};
