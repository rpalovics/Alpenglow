#include "OfflineImplicitGradientLearner.h"

vector<RecDat*> OfflineImplicitGradientLearner::generate_train_data(RecDat* rec_dat){
  vector<RecDat*> train_data;
  int number_of_positive_samples = recommender_data_->get_counter();
  train_data.reserve(number_of_positive_samples);
  for(int i=0; i<=number_of_positive_samples;i++){
    train_data.push_back(recommender_data_->get(i));
  }
  return train_data;
}

