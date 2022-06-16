#include "./common_maphandler.h"
/*
std::vector<std::vector<cell_t>> generateMap(){
    //
    int dim_x;
    int dim_y;
    //  Count lines
    std::ifstream myFile;
    std::string line;
    int lines;
    myFile.open(path);
    for(lines = 0; std::getline(myFile,line); lines++);
    dim_y = lines;
    std::vector<std::string> contents;
    this->readInput(contents);
    dim_x = contents.size();
    while(this->readInput(contents)){
        for (size_t i = 0 ; i < dim_x ; i++)
            cells[i][j]= (cell_t) type;        
    }
}

    std::string MapHandler::remove_spaces(std::string old_string) {
        std::string new_string;
        for (size_t i=0 ; i < old_string.size() ; i++) {
            if (old_string[i] == ' ') {
                if (i == 0 || i == (old_string.size() - 1)) {
                    i++;
                    continue;
                }
                while (old_string[i+1] == ' ')
                    i++;
            }
            new_string.push_back(old_string[i]);
        }
        return new_string;
    }

    std::vector<std::string> MapHandler::split_line(
        std::string token,
        char delimiter) {
        std::vector<std::string> split;
        std::istringstream input_string(token);
        while (std::getline(input_string, token, delimiter)) {
            if (!token.empty())
                split.push_back(token);
        }
        return split;
    }


    MapHandler::MapHandler(std::string filename) {
        this->text_file.open(filename);
    }

    MapHandler::~MapHandler() {
        this->text_file.close();
    }

    bool MapHandler::readInput(std::vector<std::string> & contents) {
    std::string input_line;
    std::getline(this->text_file, input_line);      //  Read line from file
    if (this->text_file.eof())                      //  Return if end of file
        return false;
    input_line = this->remove_spaces(input_line);   //  Remove redundant spaces
    contents = this->split_line(input_line, ' ');   //  Split the line
    return true;
    }
    */