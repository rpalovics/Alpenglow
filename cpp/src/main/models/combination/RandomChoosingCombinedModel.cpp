#include "RandomChoosingCombinedModel.h"
void RandomChoosingCombinedModel::add(RecDat* rec_dat){
  for(auto model:models_){
    model->add(rec_dat);
  }
}
double RandomChoosingCombinedModel::prediction(RecDat* rec_dat){
  if(rec_dat->id != last_id_ or rec_dat->user!=last_user_ or rec_dat->time !=last_timestamp_){
    active_model_id_ = random_->get_discrete(distribution_);
    active_model_ = models_[active_model_id_];
    last_id_ = rec_dat->id;
    last_user_ = rec_dat->user;
    last_timestamp_ = rec_dat->time;
  }
  return active_model_->prediction(rec_dat);
}
void RandomChoosingCombinedModel::write(ostream& file){
  for(auto model:models_){
    model->write(file);
  }
}
void RandomChoosingCombinedModel::read(istream& file){
  for(auto model:models_){
    model->read(file);
  }
}
