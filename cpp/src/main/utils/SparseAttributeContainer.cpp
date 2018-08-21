#include "SparseAttributeContainer.h"
#include <fstream>
#include <sstream>
#include <iostream>

const unordered_map<int, double>& SparseAttributeContainer::get_attributes(int id){
  auto loc = attributes_.find(id);
  if(loc != attributes_.end()){
    return loc->second;
  } else {
    return empty_map_;
  }
}

void SparseAttributeContainer::add_row(int id, vector<int> indexes, vector<double> values){
  unordered_map<int, double> attribute_row;
  int maxind = -1;
  for(uint i=0; i<indexes.size(); i++){
    attribute_row.insert({indexes[i], values[i]});
    if(indexes[i] > maxind){
      maxind = indexes[i];
    }
  }
  attributes_.insert({id, attribute_row});
  if(maxind > max_attribute_index_){
    max_attribute_index_ = maxind;
  }
}

void FileSparseAttributeContainer::load_from_file(string file_name){
  max_attribute_index_ = -1;
  attributes_.clear();
  std::ifstream ifs (file_name, std::ifstream::in);
  string line, index, value;
  vector<int> indexes;
  vector<double> values;
  int i=0;
  while(std::getline(ifs, line))
  {
    indexes.clear();
    values.clear();
    istringstream ss(line);
    while (!ss.eof())
    {
      getline( ss, index, ':' );
      getline( ss, value, ' ' );
      indexes.push_back(stoi(index));
      values.push_back(stod(value));
    }
    add_row(i, indexes, values);
    i++;
  }
}