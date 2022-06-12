#include "textfilehandler.h"

std::string TextFileHandler::remove_spaces(std::string old_string) {
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

std::vector<std::string> TextFileHandler::split_line(
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


TextFileHandler::TextFileHandler(std::string filename) {
        this->text_file.open(filename);
}

TextFileHandler::~TextFileHandler() {
        this->text_file.close();
}

bool TextFileHandler::readInput(std::vector<std::string>& contents) {
    std::string input_line;
    std::getline(this->text_file, input_line);      //  Read line from file
    if (this->text_file.eof())                      //  Return if end of file
        return false;
    input_line = this->remove_spaces(input_line);   //  Remove redundant spaces
    contents = this->split_line(input_line, ' ');   //  Split the line
    return true;
}
