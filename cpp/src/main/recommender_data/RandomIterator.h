#ifndef RANDOM_ITERATOR
#define RANDOM_ITERATOR

#include "RecommenderData.h"
#include <gtest/gtest_prod.h>
#include "RecommenderDataIterator.h"
#include "../utils/Util.h"
#include "../utils/Random.h"
#include <algorithm>

using namespace std;

//SIP_AUTOCONVERT

struct RandomIteratorParameters{
  int seed;
  bool shuffle = true;
};

class RandomIterator : public RecommenderDataIterator {
  public:
    RandomIterator(RecommenderData* recommender_data, int seed, bool shuffle);
    RandomIterator(RandomIteratorParameters* params){
      seed_ = params->seed;
      shuffle_ = params->shuffle;
    }
    RecDat* next();
    RecDat* get(int index) const override;
    RecDat* get_future(int index) const override;
    double get_following_timestamp() const override;
    RecDat* get_actual() override;
    void restart(){
      if(shuffle_) {
        random_shuffle(shuffled_data_.begin(),shuffled_data_.end(),random_);
      }
      counter_ = 0;
    }
  private:
    bool autocalled_initialize() override;
    vector<RecDat*> shuffled_data_;
    int seed_;
    Random random_;
    bool shuffle_;
    bool parent_is_initialized_ = false;
};


#endif
