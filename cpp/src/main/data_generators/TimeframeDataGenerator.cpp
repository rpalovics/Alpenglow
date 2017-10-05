#include "TimeframeDataGenerator.h"

RecommenderData* TimeframeDataGenerator::generate_recommender_data(RecDat*){
  local_recommender_data_.clear();
  double now = recommender_data_iterator_->get_actual()->time;
  double timeframe_begin=now-time_frame_length_;
  vector<RecDat>* train_data = local_recommender_data_.get_rec_data();
  for(int counter = recommender_data_iterator_->get_counter();counter>=0;counter--){
    RecDat* rec_dat = recommender_data_iterator_->get(counter);
    if(rec_dat->time<=timeframe_begin) break;
    train_data->push_back(*rec_dat);
  }
  return &local_recommender_data_;
}
