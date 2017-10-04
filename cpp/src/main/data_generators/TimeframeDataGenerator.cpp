#include "TimeframeDataGenerator.h"

RecommenderData* TimeframeDataGenerator::generate_recommender_data(RecDat*){
  local_recommender_data_.clear();
  //TODO
  //int timeframe_begin=start_time_+period_num_*period_length_-timeframe_length_;
  //vector<RecDat*> train_data;
  //for(int counter = recommender_data_->get_counter();counter>=0;counter--){
  //  RecDat* rec_dat = recommender_data_->get(counter);
  //  if(rec_dat->time<=timeframe_begin) break;
  //  train_data.push_back(rec_dat);
  //}
  //return train_data;
  return &local_recommender_data_;
}
