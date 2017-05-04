#ifndef RANDOM_ITERATOR
#define RANDOM_ITERATOR
#include "RecommenderData.h"
#include "../utils/Random.h"

class RandomIterator{
  public:
    RandomIterator(RecommenderData* recommender_data, int seed){
      shuffled_data_.resize(recommender_data->size());
      for(int i=0;i<recommender_data->size();i++){
        shuffled_data_[i]=recommender_data->get(i);
      }
      random_.set(seed);
      counter_=0;
    };
    void shuffle(){
      counter_=0;
      random_shuffle(shuffled_data_.begin(),shuffled_data_.end(),random_);
    };
    RecDat* next(){
      return shuffled_data_[counter_++];
    };
    bool hasNext(){
      return counter_ < shuffled_data_.size();
    };
    void init(){
      counter_ = 0;
    };
  private:
    Random random_;
    vector<RecDat*> shuffled_data_;
    uint counter_;
};


#endif
