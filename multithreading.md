El monitor debería contener el arreglo de elementos y el tablero.

    class BoardMonitor {
    private:
     Board board;    
     std::mutex board_mutex;     
    public:
     std::vector<Game> & getGames();    
     bool addGame(const Game & game);
     bool contains(std::string const & name);
     Game & get(std::string const & name);
     void newPlayer(std::string game_name,house_t house);
    };

    class ElementsMonitor {
        private:
        std::map<int,Selectable> elements;
        std::mutex elements_mutex;
    }

    