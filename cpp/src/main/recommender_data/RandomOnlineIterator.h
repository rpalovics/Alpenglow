#ifndef RANDOM_ONLINE_ITERATOR
#define RANDOM_ONLINE_ITERATOR

//SIP_AUTOCONVERT

#include <string>
#include <exception>
#include "RecommenderData.h"
#include "RecommenderDataIterator.h"
#include "../utils/Random.h"

struct RandomOnlineIteratorParameters{
  int seed;
};
class RandomOnlineIterator : public RecommenderDataIterator {
  public:
    RandomOnlineIterator(RandomOnlineIteratorParameters* params){
      random_.set(params->seed);
    }
    ~RandomOnlineIterator(){
      for(RecDat* rec_dat : shuffled_data_){
        delete rec_dat;
      }
    }
    RecDat* next();
    RecDat* get_actual();
    RecDat* get(int index) const;
    RecDat* get_future(int index) const;
    double get_following_timestamp() const;
  private:
    Random random_;
    vector<RecDat*> shuffled_data_; //RecDat* for const correctness argh.

    bool autocalled_initialize() override;
    bool parent_is_initialized_ = false;
};


#endif
