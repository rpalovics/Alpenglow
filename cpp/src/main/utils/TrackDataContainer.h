#ifndef TRACK_DATA_CONTAINER_H
#define TRACK_DATA_CONTAINER_H

#include "StringSplitter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct TrackDataContainerParameters {
  std::string file_name = "";
};

class TrackDataContainer {
 public:
  TrackDataContainer(TrackDataContainerParameters* p) {
    std::cerr << "Track Data Conatiner read data from " << p->file_name << std::endl;
    read_file(p->file_name.c_str());
  }
  std::vector<std::vector<int> > get_data(int id) {
    if ((int)track_attributes_.size() < id) {
      track_attributes_.resize(id+1);
      return std::vector<std::vector<int>>{};
    }
    auto ret_val = track_attributes_[id];
    if (ret_val == nullptr ) {
      return std::vector<std::vector<int>>{};
    }
    return *ret_val;
  }
 protected:
  virtual void read_file(const char *);
  virtual void process_line(std::string &line);
  std::vector<std::vector<std::vector<int> >*> track_attributes_;
};


#endif
