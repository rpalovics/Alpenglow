#include "SvdppModelGradientUpdater.h"


//void SvdppModelGradientUpdater::update(vector<RecDat>* sample) {
  //int user = sample->at(0).user;
  //vector<double> diff(model_->dimension_);
  //for (uint i=0; i<sample->size(); i++){
  //  RecDat *rec_dat = &(sample->at(i));
  //  double coeff = rec_dat->score-model_->prediction(rec_dat);
  //  Util::sum_update_with(&diff, model_->item_factors_->get(rec_dat->item), coeff);
  //  model_->item_factors_->add(rec_dat->item,model_->user_factors_->get(user),coeff*learning_rate_);	
  //}
  //Util::multiply_vector(learning_rate_,&diff);
  //model_->user_factors_->add(user,&diff);
  //
  //if (model_->user_history_.size()<=user or model_->user_history_[user]==NULL) return;
  //int user_history_length = model_->user_history_[user]->size();	
  //double weight=model_->norm_cache_;
  //for (uint i = 0; i < user_history_length; i++) {
  //  model_->history_factors_->add(model_->user_history_[user]->at(user_history_length-1-i),&diff,weight);
  //  weight *= model_->history_weight_;
  //}
//}

void SvdppModelGradientUpdater::beginning_of_updating_cycle(RecDat* rec_dat){
  if(cumulative_item_updates_){
    Util::zero_out_vector(&cumulated_histvector_updates_);
  }
}
void SvdppModelGradientUpdater::update(RecDat* rec_dat, double gradient){ 
  if(model_->use_sigmoid_){
    double pred = model_->prediction(rec_dat);
    gradient = gradient * Util::sigmoid_derivative_function(pred);
  }
  update_user_factors(rec_dat,gradient);
  vector<double>* item_vector = model_->item_factors_.get(rec_dat->item);
  if(cumulative_item_updates_){
    Util::sum_update_with(&cumulated_histvector_updates_,item_vector,gradient);
  } else {
    update_history_item_factors(rec_dat,gradient,item_vector);
    model_->invalidate_user_factor_ = true;
  }
  update_item_factors(rec_dat,gradient);
}
void SvdppModelGradientUpdater::end_of_updating_cycle(RecDat* rec_dat){
  //cerr << "train avg " << *rec_dat << " " << " mse=" << avg_mse_/n_of_s_ << endl; //DEBUG
  if(cumulative_item_updates_){
    update_history_item_factors(rec_dat,1,&cumulated_histvector_updates_);
    model_->invalidate_user_factor_ = true;
  }
  //double prediction = model_->prediction(rec_dat); //DEBUG
  //cerr << "after train " << *rec_dat << " " << " mse=" << (1-prediction)*(1-prediction) << endl; //DEBUG
}

void SvdppModelGradientUpdater::update_item_factors(RecDat* rec_dat, double gradient){
  //assumption: prediction was called and cached_user_factor is valid
  //assumption2: update_history_item_factors does not recompute p_u after changing q'_j-s, so we don't have to cache user vector
  model_->item_factors_.lin_combine(rec_dat->item,-gradient*learning_rate_, &model_->cached_user_factor_);
}
void SvdppModelGradientUpdater::update_user_factors(RecDat* rec_dat, double gradient){
  model_->user_factors_.lin_combine(rec_dat->user,-gradient*learning_rate_*model_->user_vector_weight_, model_->item_factors_.get(rec_dat->item));
}
void SvdppModelGradientUpdater::update_history_item_factors(RecDat* rec_dat,double gradient,vector<double>* item_vector){
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
          -gradient*learning_rate_*(*cached_weight)*model_->cached_norm_*model_->history_weight_,
          item_vector); }
  }
}

