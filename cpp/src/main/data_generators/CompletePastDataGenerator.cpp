#include "CompletePastDataGenerator.h"

RecommenderData* CompletePastDataGenerator::generate_recommender_data(RecDat*){
  local_recommender_data_.clear();
  vector<RecDat>* train_data = local_recommender_data_.get_rec_data();
  for(int counter = recommender_data_iterator_->get_counter();counter>=0;counter--){
    RecDat* rec_dat = recommender_data_iterator_->get(counter);
    train_data->push_back(*rec_dat);
  }
  if (!local_recommender_data_.initialize()) throw 0;
  return &local_recommender_data_;
}
