#ifndef SIMPLEITERATOR_H_
#define SIMPLEITERATOR_H_
/*
 * SimpleIterator.h
 * Order of rec_dats is the same as order of them in the scrobble file (no shuffling).
 */

#include "RecommenderDataIterator.h"

class SimpleIterator : public RecommenderDataIterator {
  public:
    SimpleIterator(RecommenderData *rd){
      counter_=0;
      recommender_data_=rd;
    }
    SimpleIterator(){
      counter_=0;
    }
    RecDat* next() override;
    RecDat* get(int index) const override;
    RecDat* get_actual() override;
    RecDat* get_future(int index) const override;
    double get_following_timestamp() const override;
};

#endif
