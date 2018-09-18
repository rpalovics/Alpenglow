#include "ShuffleIterator.h"

ShuffleIterator::ShuffleIterator(RecommenderData* data, int seed) {
  recommender_data_ = data;
  seed_ = seed;
  initialize();
}

RecDat* ShuffleIterator::next() {
  return shuffled_data_[counter_++];
}

RecDat* ShuffleIterator::get_actual(){
  return shuffled_data_[counter_-1];
}

RecDat* ShuffleIterator::get(int index) const {
  if(index>=counter_) throw exception();
  return shuffled_data_[index];
}

RecDat* ShuffleIterator::get_future(int index) const {
 return shuffled_data_[index];
}

bool ShuffleIterator::autocalled_initialize(){
  if(!parent_is_initialized_){
    parent_is_initialized_=RecommenderDataIterator::autocalled_initialize();
    if(!parent_is_initialized_) return false;
  }
  counter_ = 0;

  //shuffle same timestamped samples
  Random random(seed_);
  shuffled_data_.push_back(recommender_data_->get(0));
  double current_time = shuffled_data_[0]->time;
  int same_time_counter = 1;
  for (int i = 1; i<size(); i++){
    shuffled_data_.push_back(recommender_data_->get(i));
    if (shuffled_data_[i]->time>current_time){
      random.random_shuffle(shuffled_data_.end()-same_time_counter,shuffled_data_.end()-1); 
      same_time_counter=1;
      current_time=shuffled_data_[i]->time;
    } else same_time_counter++;
  }
  random.random_shuffle(shuffled_data_.end()-same_time_counter,shuffled_data_.end()); 
  return true;
}

double ShuffleIterator::get_following_timestamp() const {
  if(counter_ < recommender_data_->size()){
    RecDat* next = shuffled_data_[counter_];
    return next->time;
  } else {
    return -1;
  }
}

