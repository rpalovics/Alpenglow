#ifndef RANDOM_ONLINE_ITERATOR_H
#define RANDOM_ONLINE_ITERATOR_H

//SIP_AUTOCONVERT

#include <string>
#include <stdexcept>
#include "RecommenderData.h"
#include "RecommenderDataIterator.h"
#include "../utils/Random.h"

struct RandomOnlineIteratorParameters {
  int seed = 745578;
};
class RandomOnlineIterator : public RecommenderDataIterator {
/**
  This RecommenderDataIterator shuffles the samples keeping the timestamps
  ordered and serves them in a fixed random order. Note that the samples are
  modified, the nth sample of the random order gets the timestamp of the nth
  sample of the original data.
*/
public:
  RandomOnlineIterator(RandomOnlineIteratorParameters* params){
    random_.set(params->seed);
  }
  ~RandomOnlineIterator(){
    for(RecDat* rec_dat : shuffled_data_){
      delete rec_dat;
    }
  }
  RecDat* next() override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.next()`
  */
  RecDat* get_actual() const override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_actual()`
  */
  RecDat* get(int index) const override;
  /**
    get(int index)
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get()`
  */
  RecDat* get_future(int index) const override;
  /**
    get_future(int index)
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_future()`
  */
  double get_following_timestamp() const override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_following_timestamp()`
  */
  bool self_test(){
    return RecommenderDataIterator::self_test() && random_.self_test();
  }
private:
  Random random_;
  vector<RecDat*> shuffled_data_; //RecDat* for const correctness argh.

  bool autocalled_initialize() override;
  bool parent_is_initialized_ = false;
};


#endif /* RANDOM_ONLINE_ITERATOR_H */
