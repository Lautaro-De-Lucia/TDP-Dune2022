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
   int player_id;
   player_t faction;
   int spice;
   int c_spice;
   int energy;
   int c_energy;
   int efficiency;
   Protocol protocol;
   std::vector<bool> & reading_flags;
   ThreadSafeQueue & instruction_queue;
   Socket player_socket;
   bool finished;
   std::map<unit_t,size_t> units_to_create;
	 std::map<unit_t,size_t> unit_time;
	 std::map<unit_t,size_t> unit_creation_time;
   std::thread thread;
   std::vector<response_t> responses_buffer;

 public:    
   ClientHandler (int player_id, int init_energy, int init_spice,std::vector<bool> & ready, Socket && client_socket, ThreadSafeQueue & tsq);
   void run(); // This should receive the socket in the future
   response_t queueUnit(unit_t type);
   response_t checkCreation(unit_t type);
   void reportState(GameResources & game);
   bool isDone();
   void close();
   void sendResponses(std::vector<response_t> & responses);
   int getSpice();
   int getEnergy();
   int getID();
   player_t getFaction();
   void setSpice(int spice);
   void setEnergy(int energy);
   void notifyGameStart();
};
