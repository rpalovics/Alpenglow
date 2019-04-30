#ifndef SPARSE_ATTRIBUTE_CONTAINER_H
#define SPARSE_ATTRIBUTE_CONTAINER_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

struct SparseAttributeContainerParameters {

};

class SparseAttributeContainer {
public:
  const unordered_map<int, double>& get_attributes(int id);
  int get_max_attribute_index(){
    return max_attribute_index_;
  }
protected:
  int max_attribute_index_ = -1;
  unordered_map<int, unordered_map<int,double>> attributes_;
  void add_row(int id, vector<int> indexes, vector<double> values);
  unordered_map<int,double> empty_map_;
};

class FileSparseAttributeContainer : public SparseAttributeContainer{
public:
  void load_from_file(string file_name);
};

#endif /* SPARSE_ATTRIBUTE_CONTAINER_H */
