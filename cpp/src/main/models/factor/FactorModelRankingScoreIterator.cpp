#include "FactorModelRankingScoreIterator.h"

#include "../../utils/Util.h"
#include <iostream>

bool FactorModelRankingScoreIterator::has_next(double upper_bound){
  if(current_scores_.size() != 0){
    return true;
  }
  if(next_bucket_ == container_->buckets_end()){
    return false;
  }

  LempBucket* current_bucket = *next_bucket_;
  next_bucket_++;

  if(upper_bound <= current_bucket->get_bucket_max()*user_factor_norm_){
    current_scores_ = current_bucket->get_scores(&user_factor_);
    return true;
  } else {
    return false;
  }
}

pair<int, double> FactorModelRankingScoreIterator::get_next(){
  pair<int,double> score = current_scores_.back();
  current_scores_.pop_back();
  return score;
}
int FactorModelRankingScoreIterator::unique_items_num(){
  return container_->size();
}