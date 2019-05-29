#include "HighGradientNegativeSampleGenerator.h"

vector<int>* HighGradientNegativeSampleGenerator::generate(RecDat* rec_dat){
  samples.clear();
  positive_sample_ = *rec_dat;
  user_ = rec_dat->user;
  item_ = rec_dat->item;
  generate_all();
  choose_best();
  return &samples;
}

void HighGradientNegativeSampleGenerator::generate_all(){
  //generate full_negative_rate_ samples
  local_samples_.clear();
  int learnt = 0, available = items_->size();
  while(learnt<full_negative_rate_ && available>0){
    int idx = random_(available); //idx = rand%available;
    int item = items_->at(idx);
    if(!train_matrix_->has_value(user_,item_) && item!=item_){
      learnt++;
      positive_sample_.item = item;
      double prediction = model_->prediction(&positive_sample_);
      local_samples_.push_back(make_pair(item, prediction));
    }
    items_->at(idx)=items_->at(available-1);
    items_->at(available-1) = item;
    available--;
  }
}
void HighGradientNegativeSampleGenerator::choose_best(){
  sort(local_samples_.begin(),local_samples_.end(),sort_pair_descending_by_second<int>);
  for(int i=0;i<negative_rate_ && i<(int)local_samples_.size();i++){
    samples.push_back(local_samples_[i].first);
  }
}

