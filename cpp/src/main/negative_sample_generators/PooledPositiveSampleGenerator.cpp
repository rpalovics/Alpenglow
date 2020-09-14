#include "PooledPositiveSampleGenerator.h"

vector<RecDat>* PooledPositiveSampleGenerator::get_implicit_train_data(RecDat*
    positive_sample){
  implicit_train_data_.clear();
  if(pool_.size()==0){
    for(int i=0;i<positive_rate_;i++){
      pool_.push_back(positive_sample);
    }
  } else {
    for(int i=0;i<positive_rate_;i++){
      int random_idx=random_.get(pool_.size());
      implicit_train_data_.push_back(*pool_[random_idx]);
      if(pool_.size()<max_pool_size_){
        pool_.push_back(positive_sample);
      } else {
        pool_[random_idx]=positive_sample;
      }
    }
  }
  return &implicit_train_data_;
}
