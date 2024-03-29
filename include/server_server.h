#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <sys/socket.h>

#include "common_socket.h"
#include "server_clienthandler.h"
#include "server_gameresources.h"
#include "common_utils.h"
#include "server_tsq.h"



class Server {
 private:
      Socket socket;       ///   Accepting socket for new clients
      GameResources game;                  ///   Monitor to handle resources
      std::vector<std::unique_ptr<ClientHandler>> players;  ///   Client Handlers each of them holding a thread
      bool running;                         ///   Status flag
      ThreadSafeQueue TSQ;
      std::map<int,std::vector<creation_t>> creation_data;
      std::map<player_t,std::map<unit_t,int>> unit_creation_time;
      std::map<player_t,int> time_penalty;
      // std::map<player_t,std::map<unit_t,int>> units_to_create;
      std::map<player_t,std::map<unit_t,int>> unit_time;
      std::map<player_t,std::map<building_t,std::queue<unit_t>>> creating_queues;
      std::vector<bool> ready_flags;

 public:
      // Initialize the accepting socket
      Server(const char* service_name, std::vector<std::vector<cell_t>> cells);
      void acceptPlayers();
      void run();
      void checkForFinishedClients();
      void closeAllClients();
      void stopClientAccept();
      void stopGameLoop();
      void read_command(std::istream& input_stream);

      void handleInstruction(std::unique_ptr<instruction_t> & INS);      
      void handleInstruction(building_create_t & INS);
      void handleInstruction(unit_create_t & INS);
      void handleInstruction(left_click_t & INS);
      void handleInstruction(right_click_t & INS);
      void handleInstruction(selection_t & INS);
      void handleInstruction(idle_t & INS);
      void handleInstruction(close_t & INS);

      void updateCreationState();
      void enableReading();
      void update();

      std::unique_ptr<ClientHandler> & getPlayer(player_t faction);
      response_t checkCreation(player_t faction, building_t type);
      building_t getCreator(unit_t type);
      Position getFactionBase(player_t faction);

      void run2();
};
