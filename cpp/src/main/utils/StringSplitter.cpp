#include "StringSplitter.h"

std::vector<std::string> split(const std::string& input, const char sep) {
  size_t temp_pos1 = 0;
  size_t temp_pos2 =  input.find(sep);
  std::vector<std::string> return_vector;
  while (temp_pos2 != std::string::npos) {
    return_vector.push_back(input.substr(temp_pos1, temp_pos2 - temp_pos1));
    temp_pos1 = temp_pos2 + 1;
    temp_pos2 = input.find(sep, temp_pos2 + 1);
  }
  return_vector.push_back(input.substr(temp_pos1, temp_pos2 - temp_pos1));
  return return_vector;
}

