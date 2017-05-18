/*
 * RecommenderDataIterator.h
 * RecommenderData should be accessed through iterator. An iterator object can shuffle order of recDats, but if the data is iterated repeatedly, the iterator must give recDats in the same order.
 */

#ifndef RECOMMENDERDATAITERATOR_H_
#define RECOMMENDERDATAITERATOR_H_

#include "RecommenderData.h"
#include "../utils/SpMatrix.h"

class RecommenderDataIterator {
public:
  void set_recommender_data(RecommenderData* data){ recommender_data_=data; }
  virtual void init(){}
  virtual bool has_next() const { return counter_ < recommender_data_->size(); } //iterator
  virtual RecDat* next() = 0; //iterator
  virtual int get_counter() const { return counter_-1; } //get the index of actual sample
  virtual RecDat* get(int index) const = 0; //index'th sample from recommender_data
  virtual RecDat* get_future(int index) const = 0; //index'th sample from recommender_data, getting sample from future is allowed
  virtual int size() const { return recommender_data_->size(); } //number of samples
  virtual double get_following_timestamp() const = 0; //returns timestamp of the following sample, returns -1 in case of last sample
  virtual ~RecommenderDataIterator() {}
protected:
  RecommenderData* recommender_data_;
  int counter_; //index of next element
};


#endif /* IRECOMMENDERDATAITERATOR_H_ */
