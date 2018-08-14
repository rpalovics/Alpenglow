#ifndef RANDOM_ITERATOR
#define RANDOM_ITERATOR

//SIP_AUTOCONVERT

#include <string>
#include <exception>
#include "RecommenderData.h"
#include "RecommenderDataIterator.h"
#include "../utils/Random.h"

struct RandomIteratorParameters{
  int seed;
  string shuffle_mode = "auto_shuffle";
};
class RandomIterator : public RecommenderDataIterator {
  public:
    RandomIterator(RecommenderData* recommender_data, int seed, string shuffle_mode){
      recommender_data_ = recommender_data;
      random_.set(seed);
      shuffle_mode_ = shuffle_mode;
      if (!initialize()){
        cerr << "RandomIterator: initialization was no successful." << endl;
        throw 0;
      }
    }
    RandomIterator(RandomIteratorParameters* params){
      random_.set(params->seed);
      shuffle_mode_=params->shuffle_mode;
    }
    RecDat* next();
    void restart();
    void shuffle();
    RecDat* get_actual();
    RecDat* get(int index) const;
    RecDat* get_future(int index) const;
    double get_following_timestamp() const;
  private:
    Random random_;
    vector<RecDat*> shuffled_data_;
    string shuffle_mode_ = "auto_shuffle"; //auto_shuffle or manual_shuffle

    bool autocalled_initialize() override;
    bool parent_is_initialized_ = false;
};


#endif
