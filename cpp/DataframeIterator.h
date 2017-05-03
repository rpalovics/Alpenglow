#ifndef DATAFRAMEITERATOR_H
#define DATAFRAMEITERATOR_H

#include "recommender_data/RecommenderDataIterator.h"
#include <vector>

using namespace std;

class DataframeIterator : public RecommenderDataIterator {
public:
  DataframeIterator(){
    counter_ = 0;
  }
  void add_recdat(RecDat &recDat){
    recdats_.push_back(new RecDat(recDat));
  }
  virtual bool has_next() const {
    return counter_ < recdats_.size(); 
  }
  virtual RecDat* next() {
    return get_future(counter_++);
  };
  virtual int get_counter() const { return counter_-1; }
  virtual RecDat* get(int index) const {
    if(index>=counter_) throw exception();
    return get_future(index);
  }
  virtual RecDat* get_future(int index) const { return recdats_[index]; }
  virtual int size() const { return recdats_.size(); }
  virtual double get_following_timestamp() const {
    if(counter_ < recdats_.size()){
      const RecDat* next = get_future(counter_);
      return next->time;
    } else {
      return -1;
    }
  }
  virtual ~DataframeIterator() {
    for(auto it : recdats_){
      delete it;
    }
  }
protected:
  long counter_;
  vector<RecDat*> recdats_;
};

#endif