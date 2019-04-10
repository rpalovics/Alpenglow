#include "Model.h"

#include <fstream>

void Model::write(string file_name){
  ofstream file(file_name);
  write(file);
  file.close();
}

void Model::read(string file_name){
  ifstream file(file_name);
  read(file);
  file.close();
}
