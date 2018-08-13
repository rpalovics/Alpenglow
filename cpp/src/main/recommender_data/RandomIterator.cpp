#include "RandomIterator.h"

RandomIterator::RandomIterator(RecommenderData* data, int seed, bool shuffle) {
  recommender_data_ = data;
  seed_ = seed;
  shuffle_ = shuffle;
  initialize();
}

RecDat* RandomIterator::next() {
  return shuffled_data_[counter_++];
}

RecDat* RandomIterator::get_actual(){
  return shuffled_data_[counter_-1];
}

RecDat* RandomIterator::get(int index) const {
  if(index>=counter_) throw exception();
  return shuffled_data_[index];
}

RecDat* RandomIterator::get_future(int index) const {
 return shuffled_data_[index];
}

bool RandomIterator::autocalled_initialize(){
  if(!parent_is_initialized_){
    parent_is_initialized_=RecommenderDataIterator::autocalled_initialize();
    if(!parent_is_initialized_) return false;
  }
  counter_ = 0;

  //shuffle same timestamped samples
  shuffled_data_.resize(recommender_data_->size());
  for(int i=0;i<recommender_data_->size();i++){
    shuffled_data_[i]=recommender_data_->get(i);
  }
  random_.set(seed_);
  if(shuffle_) {
    random_shuffle(shuffled_data_.begin(),shuffled_data_.end(),random_);
  }
  return true;
}

double RandomIterator::get_following_timestamp() const {
  if(counter_ < recommender_data_->size()){
    RecDat* next = shuffled_data_[counter_];
    return next->time;
  } else {
    return -1;
  }
}

