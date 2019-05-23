#include "RandomChoosingCombinedModel.h"
void RandomChoosingCombinedModel::add(RecDat* rec_dat){
  for(auto model:wms_.models_){
    model->add(rec_dat);
  }
}
double RandomChoosingCombinedModel::prediction(RecDat* rec_dat){
  if(rec_dat->id != last_id_ or rec_dat->user!=last_user_ or rec_dat->time !=last_timestamp_){
    active_model_id_ = random_.get_discrete(wms_.distribution_);
    active_model_ = wms_.models_[active_model_id_];
    last_id_ = rec_dat->id;
    last_user_ = rec_dat->user;
    last_timestamp_ = rec_dat->time;
  }
  return active_model_->prediction(rec_dat);
}
void RandomChoosingCombinedModel::write(ostream& file){
  for(auto model:wms_.models_){
    model->write(file);
  }
}
void RandomChoosingCombinedModel::read(istream& file){
  for(auto model:wms_.models_){
    model->read(file);
  }
}

RankingScoreIterator* RandomChoosingCombinedModel::get_ranking_score_iterator(int user){
  if(user!=last_user_) throw runtime_error("prediction should be called first, the parameter here is user, not recdat, we can't update state fields properly");
  if(rsip_models_[active_model_id_]==NULL) return NULL; //the actual model is not an rsip model
  return rsip_models_[active_model_id_]->get_ranking_score_iterator(user);
}
