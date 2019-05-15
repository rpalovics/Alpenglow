#include "RandomIterator.h"

RecDat* RandomIterator::get_actual() const {
  return shuffled_data_[counter_-1];
}

RecDat* RandomIterator::get(int index) const {
  if(index>=counter_) throw runtime_error("Attempted to access future.");
  return shuffled_data_[index];
}

RecDat* RandomIterator::get_future(int index) const {
 return shuffled_data_[index];
}
RecDat* RandomIterator::next(){
  return shuffled_data_[counter_++];
};
void RandomIterator::restart(){
  if(shuffle_mode_ == "auto_shuffle") shuffle();
  counter_ = 0;
};
void RandomIterator::shuffle() {
  random_.random_shuffle(shuffled_data_.begin(),shuffled_data_.end());
};
double RandomIterator::get_following_timestamp() const {
  if(counter_ < recommender_data_->size()){
    RecDat* next = shuffled_data_[counter_];
    return next->time;
  } else {
    return -1;
  }
}

bool RandomIterator::autocalled_initialize() {
  if(!parent_is_initialized_){
    parent_is_initialized_=RecommenderDataIterator::autocalled_initialize();
    if(!parent_is_initialized_) return false;
  }
  shuffled_data_.resize(recommender_data_->size());
  for(int i=0;i<recommender_data_->size();i++){
    shuffled_data_[i]=recommender_data_->get(i);
  }
  restart();
  return true;
}
