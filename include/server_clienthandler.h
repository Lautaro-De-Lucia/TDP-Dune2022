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

class ClientHandler {
 private:
   int player_id;
   player_t faction;
   int spice;
   int c_spice;
   int energy;
   int c_energy;
   int construction_wait;
   int efficiency;
   Protocol protocol;
   std::vector<bool> & reading_flags;
   ThreadSafeQueue & instruction_queue;
   Socket player_socket;
   bool finished;
   std::thread thread;
   std::vector<response_t> responses_buffer;
   std::vector<creation_t> creation_data;

 public:    
   ClientHandler (int player_id, int init_energy, int init_spice,std::vector<bool> & ready, Socket && client_socket, ThreadSafeQueue & tsq);
   void run(); // This should receive the socket in the future
   response_t queueUnit(unit_t type);
   response_t checkCreation(unit_t type);
   void reportState(GameResources & game);
   bool isDone();
   void close();
   void sendResponses();
   void setCreationData(std::vector<creation_t> & creation_data);
   void sendCreationData();
   int & getSpice();
   int & getEnergy();
   int & getConstructionWait();
   int getID();
   player_t getFaction();
   void setSpice(int spice);
   void setEnergy(int energy);
   void notifyGameStart();
   void run2();
   void pushResponse(response_t response);

};
