#include "AsymmetricFactorModelGradientUpdater.h"
void AsymmetricFactorModelGradientUpdater::beginning_of_updating_cycle(RecDat* rec_dat){
  //double prediction = model_->prediction(rec_dat); //DEBUG
  //cerr << "before train " << *rec_dat << " " << " mse=" << setprecision(8) << (1-prediction)*(1-prediction) << endl; //DEBUG
  //avg_mse_=0; //DEBUG
  //n_of_s_=0; //DEBUG
  if(cumulative_item_updates_){
    Util::zeroOutVector(&cumulated_histvector_updates_);
  }
}
void AsymmetricFactorModelGradientUpdater::update(RecDat* rec_dat, double gradient){ 
  //double prediction = model_->prediction(rec_dat); //DEBUG
  //double score = rec_dat->score; //DEBUG
  //avg_mse_+=(score-prediction)*(score-prediction); //DEBUG
  //n_of_s_++; //DEBUG
  if(model_->use_sigmoid_){
    double pred = model_->prediction(rec_dat);
    gradient = gradient * Util::sigmoidDerivativeFunction(pred);
  }
  vector<double>* item_vector = model_->item_factors_.get(rec_dat->item);
  if(cumulative_item_updates_){
    Util::sumUpdateWith(&cumulated_histvector_updates_,item_vector,gradient);
  } else {
    update_history_item_factors(rec_dat,gradient,item_vector);
    model_->invalidate_user_factor_ = true;
  }
  update_item_factors(rec_dat,gradient);
}
void AsymmetricFactorModelGradientUpdater::end_of_updating_cycle(RecDat* rec_dat){
  //cerr << "train avg " << *rec_dat << " " << " mse=" << avg_mse_/n_of_s_ << endl; //DEBUG
  if(cumulative_item_updates_){
    update_history_item_factors(rec_dat,1,&cumulated_histvector_updates_);
    model_->invalidate_user_factor_ = true;
  }
  //double prediction = model_->prediction(rec_dat); //DEBUG
  //cerr << "after train " << *rec_dat << " " << " mse=" << (1-prediction)*(1-prediction) << endl; //DEBUG
}

void AsymmetricFactorModelGradientUpdater::update_item_factors(RecDat* rec_dat, double gradient){
  //assumption: prediction was called and cached_user_factor is valid
  //assumption2: update_history_item_factors does not recompute p_u after changing q'_j-s, so we don't have to cache user vector
  model_->item_factors_.lin_combine(rec_dat->item,-gradient*learning_rate_, &model_->cached_user_factor_);
}
void AsymmetricFactorModelGradientUpdater::update_history_item_factors(RecDat* rec_dat,double gradient,vector<double>* item_vector){
  //assumption: prediction was called and cached_weights is valid
  const vector<const RecDat*>* user_history = model_->user_history_container_.get_user_history(rec_dat->user);
  if(user_history != NULL){
    auto history_iterator = user_history->rbegin();
    auto cached_weight = model_->cached_weights_.begin();
    for(;
        history_iterator!=user_history->rend() && cached_weight!=model_->cached_weights_.end();
        history_iterator++,cached_weight++)
    {
      model_->history_item_factors_.lin_combine(
          (*history_iterator)->item,
          -gradient*learning_rate_*(*cached_weight)*model_->cached_norm_,
          item_vector); }
  }
}
