#include "PosNegFactorModelGradientUpdater.h"

void PosNegFactorModelGradientUpdater::update(RecDat* rec_dat, double gradient){
  if(model_->use_sigmoid_){
    double pred = model_->prediction(rec_dat);
    gradient *= Util::sigmoidDerivativeFunction(pred);
  }
  update_biases(rec_dat,gradient);
  update_factors(rec_dat,gradient); 
}

void PosNegFactorModelGradientUpdater::update_biases(RecDat* rec_dat, double gradient){
  double local_learning_rate = learning_rate_bias_;
  if(rec_dat->eval>=2) local_learning_rate*=learning_rate_decrease_;
  if(model_->use_user_bias_ and !turn_off_user_bias_updates_ and (rec_dat->eval<2 or turn_on_user_posneg_bias_updates_)){
     double user_gradient = gradient;
     if(model_->user_recency_!=NULL) user_gradient*=model_->user_recency_->get(rec_dat->user,rec_dat->time);
     model_->user_bias_.update(rec_dat->user,-user_gradient*local_learning_rate);
     model_->user_bias_.update(rec_dat->user,-regularization_rate_bias_*local_learning_rate);
  }
  if(model_->use_item_bias_ and !turn_off_item_bias_updates_ and (rec_dat->eval<2 or turn_on_item_posneg_bias_updates_)){
     double item_gradient = gradient;
     if(model_->item_recency_!=NULL) item_gradient*=model_->item_recency_->get(rec_dat->item,rec_dat->time);
     model_->item_bias_.update(rec_dat->item,-item_gradient*local_learning_rate);
     model_->item_bias_.update(rec_dat->item,-regularization_rate_bias_*local_learning_rate);
  }
}

void PosNegFactorModelGradientUpdater::update_factors(RecDat* rec_dat, double gradient){ 
  if(model_->user_recency_!=NULL) gradient*=model_->user_recency_->get(rec_dat->user,rec_dat->time);
  if(model_->item_recency_!=NULL) gradient*=model_->item_recency_->get(rec_dat->item,rec_dat->time);
  double local_learning_rate = learning_rate_;
  if(rec_dat->eval>=2) local_learning_rate = learning_rate_decrease_*learning_rate_;
  vector<double> tmp_user_factor = *model_->user_factors_.get(rec_dat->user);
  vector<double> tmp_item_factor = *model_->item_factors_.get(rec_dat->item);
  if(!turn_off_user_factor_updates_ and (rec_dat->eval<2 or turn_on_user_posneg_factor_updates_)){
    model_->user_factors_.linCombine(rec_dat->user,-gradient*local_learning_rate, &tmp_item_factor); //update
    model_->user_factors_.linCombine(rec_dat->user,-regularization_rate_*local_learning_rate, &tmp_user_factor); //reg
  }
  if(!turn_off_item_factor_updates_ and (rec_dat->eval<2 or turn_on_item_posneg_factor_updates_)){
    model_->item_factors_.linCombine(rec_dat->item,-gradient*local_learning_rate, &tmp_user_factor); //update
    model_->item_factors_.linCombine(rec_dat->item,-regularization_rate_*local_learning_rate, &tmp_item_factor); //reg
  }
}
