#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>


class TextFileHandler {
 private:
   std::ifstream text_file;

   std::string remove_spaces(std::string old_string);
   std::vector<std::string> split_line(std::string token, char delimiter);

 public:
   explicit TextFileHandler(std::string filename);
   ~TextFileHandler();
   bool readInput(std::vector<std::string>& contents);
};
