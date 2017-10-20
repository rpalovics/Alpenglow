#include "FactorModelGradientUpdater.h"

void FactorModelGradientUpdater::update(RecDat* rec_dat, double gradient){
  //cout << "update " << *rec_dat << " grad=" << gradient << endl;
  if(model_->use_sigmoid_){
    double pred = model_->prediction(rec_dat);
    gradient *= Util::sigmoid_derivative_function(pred);
  }
  update_biases(rec_dat,gradient);
  update_factors(rec_dat,gradient);
  
  model_->lemp_container_.schedule_update_item(rec_dat->item);
}

void FactorModelGradientUpdater::update_biases(RecDat* rec_dat, double gradient){
  if(model_->use_user_bias_ and !turn_off_user_bias_updates_){
     double user_gradient = gradient;
     if(model_->user_recency_!=NULL) user_gradient*=model_->user_recency_->get(rec_dat->user,rec_dat->time);
     model_->user_bias_.update(rec_dat->user,-user_gradient*learning_rate_bias_);
     model_->user_bias_.update(rec_dat->user,-regularization_rate_bias_*learning_rate_bias_);
  }
  if(model_->use_item_bias_ and !turn_off_item_bias_updates_){
    double item_gradient = gradient;
    if(model_->item_recency_!=NULL) item_gradient*=model_->item_recency_->get(rec_dat->item,rec_dat->time);
    model_->item_bias_.update(rec_dat->item,-item_gradient*learning_rate_bias_);
    model_->item_bias_.update(rec_dat->item,-regularization_rate_bias_*learning_rate_bias_);
  }
}

void FactorModelGradientUpdater::update_factors(RecDat* rec_dat, double gradient){ 
  if(model_->user_recency_!=NULL) gradient*=model_->user_recency_->get(rec_dat->user,rec_dat->time);
  if(model_->item_recency_!=NULL) gradient*=model_->item_recency_->get(rec_dat->item,rec_dat->time);
  vector<double> tmp_user_factor = *model_->user_factors_.get(rec_dat->user);
  vector<double> tmp_item_factor = *model_->item_factors_.get(rec_dat->item);
  if(!turn_off_user_factor_updates_){
    model_->user_factors_.lin_combine(rec_dat->user,-gradient*learning_rate_, &tmp_item_factor); //update
    model_->user_factors_.lin_combine(rec_dat->user,-regularization_rate_*learning_rate_, &tmp_user_factor); //reg
  }
  if(!turn_off_item_factor_updates_){
    model_->item_factors_.lin_combine(rec_dat->item,-gradient*learning_rate_, &tmp_user_factor); //update
    model_->item_factors_.lin_combine(rec_dat->item,-regularization_rate_*learning_rate_, &tmp_item_factor); //reg
  }
}
