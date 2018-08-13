#ifndef RANDOM_ITERATOR
#define RANDOM_ITERATOR
#include <string>
#include "RecommenderData.h"
#include "../utils/Random.h"

class RandomIterator : public Initializable {
  public:
    RandomIterator(RecommenderData* recommender_data, int seed, string shuffle_mode){
      recommender_data_ = recommender_data;
      random_.set(seed);
      shuffle_mode_ = shuffle_mode;
      initialize();
    };
    RecDat* next();
    void restart();
    void shuffle();
    RecDat* get_actual();
    RecDat* get(int index) const;
    RecDat* get_future(int index) const;
    double get_following_timestamp() const;
    //common parts TODO
    bool has_next(){
      return counter_ < shuffled_data_.size();
    };
  private:
    Random random_;
    vector<RecDat*> shuffled_data_;
    string shuffle_mode_ = "auto_shuffle"; //auto_shuffle or manual_shuffle

    //common parts TODO
    bool autocalled_initialize() override {
      //if(!parent_is_initialized_){
      //  parent_is_initialized_=RecommenderDataIterator::autocalled_initialize();
      //  if(!parent_is_initialized_) return false;
      //}
      shuffled_data_.resize(recommender_data_->size());
      for(int i=0;i<recommender_data_->size();i++){
        shuffled_data_[i]=recommender_data_->get(i);
      }
      restart();
      return recommender_data_->is_initialized();
    }
    RecommenderData* recommender_data_ = NULL;
    uint counter_;
};


#endif
