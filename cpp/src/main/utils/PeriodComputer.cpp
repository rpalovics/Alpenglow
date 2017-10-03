#include "PeriodComputer.h"
bool PeriodComputer::end_of_period() const {
  return timestamp_ > start_time_ and next_period_num_ != period_num_;
}
void PeriodComputer::update(RecDat* rec_dat){ //TODO cleanup
  double following_timestamp;
  if ( mode_=="time" ){
    following_timestamp = recommender_data_iterator_->get_following_timestamp();
    if(following_timestamp==-1){
      if(!reached_end_){
        next_period_num_ = -1;
        reached_end_ = true;
      }
      else{
        next_period_num_ = period_num_; //TODO use end_of_period variable?
      }
    }
    int counter = recommender_data_iterator_->get_counter();
    timestamp_ = recommender_data_iterator_->get(counter)->time;
    period_num_ = timestamp_/period_length_; //TODO blah.
  }
  if ( mode_=="samplenum" ){
    timestamp_ = recommender_data_iterator_->get_counter();
    if(timestamp_==recommender_data_iterator_->size()-1){
      if(!reached_end_){
        next_period_num_ = -1;
        reached_end_ = true;
      }
      else{
        next_period_num_ = period_num_; //TODO use end_of_period variable?
      }
    }
    int counter = recommender_data_iterator_->get_counter();
    period_num_ = counter/period_length_;
    following_timestamp = counter+1;
  }
  if(!reached_end_){
    next_period_num_ = following_timestamp/period_length_;
  }
}
int PeriodComputer::get_period_num() const {
  return period_num_;
}
