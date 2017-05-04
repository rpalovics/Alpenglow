#include "ShuffleIterator.h"

ShuffleIterator::ShuffleIterator(RecommenderData* data, int seed) {
  recommender_data_ = data;
  seed_ = seed;
  init();
}

RecDat* ShuffleIterator::next() {
  return shuffled_data_[counter_++];
}

RecDat* ShuffleIterator::get(int index) const {
  if(index>=counter_) throw exception();
  return shuffled_data_[index];
}

RecDat* ShuffleIterator::get_future(int index) const {
 return shuffled_data_[index];
}

void ShuffleIterator::init(){ 
  counter_ = 0;
  //initialize shuffled data
  Random random(seed_);
  shuffled_data_.push_back(recommender_data_->get(0));
  double current_time = shuffled_data_[0]->time;
  int same_time_counter = 1;
  for (int i = 1; i<size(); i++){
    shuffled_data_.push_back(recommender_data_->get(i));
    if (shuffled_data_[i]->time>current_time){
      random_shuffle(shuffled_data_.end()-same_time_counter,shuffled_data_.end()-1,random); 
      same_time_counter=1;
      current_time=shuffled_data_[i]->time;
    } else same_time_counter++;
  }
  random_shuffle(shuffled_data_.end()-same_time_counter,shuffled_data_.end(),random); 
}

double ShuffleIterator::get_following_timestamp() const {
  if(counter_ < recommender_data_->size()){
    RecDat* next = shuffled_data_[counter_];
    return next->time;
  } else {
    return -1;
  }
}

