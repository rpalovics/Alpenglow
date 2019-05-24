#include "UniformNegativeSampleGenerator.h"

vector<int>* UniformNegativeSampleGenerator::generate(RecDat* rec_dat){
  if(!filter_repeats_){
    int learnt = 0;
    samples.clear();
    int user_activity = train_matrix_->row_size(rec_dat->user);
    while(learnt < negative_rate_ && learnt<(int)items_->size()-user_activity){
      int item = items_->at((int)(random_.get()*(items_->size())));
      if(!train_matrix_->has_value(rec_dat->user,item)){
        learnt++;
        samples.push_back(item);
      }
    }  
    return &samples;
  } else { //no repeating items in the negative sample set
    for(uint i=indices_.size();i<items_->size();i++){
      indices_.push_back(i);
    }
    int number_of_generated = 0;
    int available = items_->size(); //==indices_.size()
    samples.clear();
    while(number_of_generated < negative_rate_ && available>0){
      int idx_idx = ((int)(random_.get()*available));
      int idx = indices_[idx_idx];
      int item = items_->at(idx);
      if(!train_matrix_->has_value(rec_dat->user,item)){
        number_of_generated++;
        samples.push_back(item);
      }
      indices_[idx_idx]=indices_[available-1];
      indices_[available-1] = idx;
      available--;
    }  
    return &samples;
  }
}

