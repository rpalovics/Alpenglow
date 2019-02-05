#include "PeriodComputer.h"
bool PeriodComputer::end_of_period() const {
  return timestamp_ > start_time_ and end_of_period_;
}
void PeriodComputer::update(RecDat*){
  double following_timestamp;
  bool middle_of_experiment = true;
  if ( period_mode_=="time" ){
    timestamp_ = recommender_data_iterator_->get_actual()->time;
    following_timestamp = recommender_data_iterator_->get_following_timestamp();
    middle_of_experiment = (following_timestamp != -1);
  }
  if ( period_mode_=="samplenum" ){
    timestamp_ = recommender_data_iterator_->get_counter();
    following_timestamp = timestamp_+1;
    middle_of_experiment = (timestamp_!=recommender_data_iterator_->size()-1);
  }
  period_num_ = timestamp_/period_length_;
  if(middle_of_experiment){
    int next_period_num = following_timestamp/period_length_;
    end_of_period_ = (period_num_ != next_period_num);
  } else {
    end_of_period_ = !reached_end_already_;
    reached_end_already_ = true;
  }
}
int PeriodComputer::get_period_num() const {
  return period_num_+1;
}
