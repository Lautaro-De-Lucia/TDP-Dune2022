#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <sys/socket.h>

#include "./common_socket.h"
#include "./server_clienthandler.h"
#include "./server_gameresources.h"
#include "./common_utils.h"

#define INIT_SPICE 15000
#define INIT_CSPICE 20000
#define INIT_ENERGY 3000
#define INIT_CENERGY 5000

class Server {
 private:
    Socket socket;       ///   Accepting socket for new clients
    GameResources game;                  ///   Monitor to handle resources
    std::vector<ClientHandler> players;   ///   Client Handlers each of them holding a thread
    bool running;                         ///   Status flag

 public:
   ///   Initialize the accepting socket
    Server(const char* service_name, std::vector<std::vector<cell_t>> cells);
    void run();
    void checkForFinishedClients();
    void closeAllClients();
    void stop();
};
