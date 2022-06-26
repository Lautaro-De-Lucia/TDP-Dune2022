#pragma once

#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <thread>
#include <unistd.h>
#include <memory>

#include "server_selectable.h"
#include "server_buildingfactory.h"
#include "server_unitfactory.h"
#include "server_unit.h"
#include "server_gameresources.h"
#include "server_tsq.h"

#include "common_utils.h"
#include "common_socket.h"
#include "common_protocol.h"

#define ATREIDES_INIT_POS_X 10
#define ATREIDES_INIT_POS_Y 5

#define HARKONNEN_INIT_POS_X 77 
#define HARKONNEN_INIT_POS_Y 42

#define ORDOS_INIT_POS_X 5 
#define ORDOS_INIT_POS_Y 40

class ClientHandler {
 private:
   player_t faction;
   int spice;
   int c_spice;
   int energy;
   int c_energy;
   int efficiency;
   Protocol protocol;
   ThreadSafeQueue & instruction_queue;
   Socket player_socket;
   bool finished;
   std::map<unit_t,size_t> units_to_create;
	 std::map<unit_t,size_t> unit_time;
	 std::map<unit_t,size_t> unit_creation_time;
   std::thread thread;
   std::vector<response_t> responses_buffer;

 public:    
   ClientHandler (int init_energy, int init_spice ,Socket && client_socket,ThreadSafeQueue & tsq);
   void run(); // This should receive the socket in the future
   response_t createBuilding(int type, int pos_x, int pos_y);
   response_t createUnit(int type, int& spice);
   void handleLeftClick(int x, int y);
   void handleRightClick(int x, int y);
   void handleSelection(int xmin, int xmax, int ymin, int ymax);
   void update();
   response_t queueUnit(unit_t type);
   response_t checkCreation(unit_t type);
   void reportState();
   bool isDone();
   void close();
};
