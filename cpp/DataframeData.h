#ifndef DATAFRAME_DATA_H
#define DATAFRAME_DATA_H

#include "recommender_data/RecommenderData.h"
#include <vector>

using namespace std;

class DataframeData : public RecommenderData{
public:
  bool init() override {return true;}
  void add_recdats(vector<RecDat*> rds){
    for(auto i : rds){
      rec_data_.push_back(*i);
    }
  }
  int size() override {return rec_data_.size();}
  RecDat* get(int idx) override {return &(rec_data_[idx]);}
private:
  vector<RecDat> rec_data_;
};
#endif