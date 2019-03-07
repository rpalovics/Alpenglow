#ifndef LABEL_CONTAINER_H
#define LABEL_CONTAINER_H

#include<iostream>
#include<vector>

using namespace std;

#define LABEL int
#define ENTITY int

class LabelContainer{
  public:
    void read_from_file(istream& file);
    vector<ENTITY> get_entities(LABEL label);
    LABEL get_label(ENTITY entity);
  private:
    vector<LABEL> entities;
    vector<vector<ENTITY> > labels;
};

#endif /* LABEL_CONTAINER_H */
