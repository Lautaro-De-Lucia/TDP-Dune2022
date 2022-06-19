#include "./server_server.h"

extern std::map<color_t,SDL_Color> colors;

#define MAX_CONNECTIONS 1

int ID = 0;

std::vector<std::string> file_input;

Server::Server(const char* service_name, std::vector<std::vector<cell_t>> cells) : socket(service_name), game(cells) {
    this->running = false;
}

void Server::run() {
    this->running = true;
    while (running == true){
        Socket client_socket = (this->socket.accept());
        this->players.push_back(ClientHandler(HARKONNEN,INIT_ENERGY,INIT_SPICE,client_socket,&(this->game)));
        checkForFinishedClients();
    }
    closeAllClients();
}

void Server::checkForFinishedClients(){
    for (size_t k = 0; k < this->players.size(); k++){ 
        if (this->players[k].isDone() == true){
            this->players[k].close();
            this->players.erase(this->players.begin() + k);
        }
    }
}

void Server::closeAllClients() {
    for (size_t k = 0; k < this->players.size(); k++){ 
        this->players[k].close();
    }
}

// @TODO: lanzar excepción
void Server::stop() {
    running = false;
    if ((this->socket).shutdown(SHUT_RDWR))
        std::cerr << "SHUTDOWN_ERROR: " << '\n';
}
