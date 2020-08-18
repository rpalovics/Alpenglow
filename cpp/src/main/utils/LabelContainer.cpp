#include "LabelContainer.h"

void LabelContainer::read_from_file(istream& file){
  if (file.fail()){
    cerr << "LabelContainer::read file error, did not read anything." << endl;
    return;
  }
  ENTITY entity;
  LABEL label; 
  int line_number = 0;
  while(!file.eof()){
    line_number++;
    if(line_number%1000000==0) cerr << "LabelContainer: read the labels for " << line_number << " entities. " << endl;
    file >> entity >> label;
    if(file.eof()) break; //TODO test for errors
    if((int)entities.size()<=entity) entities.resize(entity+1,-1);
    entities[entity] = label;
    if((int)labels.size()<=label) labels.resize(label+1);
    labels[label].push_back(entity);
  }
}

vector<ENTITY> LabelContainer::get_entities(LABEL label){
  bool out_of_range = (int) labels.size() <= label or label < 0;
  if (out_of_range) return vector<int>();
  else return labels[label];
}

LABEL LabelContainer::get_label(ENTITY entity){
  bool out_of_range = (int) entities.size() <= entity or entity < 0;
  if (out_of_range) return -1;
  else return entities[entity];
}
