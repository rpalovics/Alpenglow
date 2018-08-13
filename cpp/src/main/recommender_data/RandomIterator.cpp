#include "RandomIterator.h"

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
RecDat* RandomIterator::next(){
  return shuffled_data_[counter_++];
};
void RandomIterator::restart(){
  if(shuffle_mode_ == "auto_shuffle") shuffle();
  counter_ = 0;
};
void RandomIterator::shuffle() {
  random_shuffle(shuffled_data_.begin(),shuffled_data_.end(),random_);
};
