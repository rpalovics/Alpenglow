#include "SvdppModelGradientUpdater.h"

void SvdppModelGradientUpdater::update(RecDat* rec_dat, double gradient){ 
  if (cumulative_item_updates_ && first_sample_==NULL) first_sample_=rec_dat;
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
  }
  update_item_factors(rec_dat,gradient);
  if(!cumulative_item_updates_){
    model_->cache_marked_invalid_ = true;
  }
}

void SvdppModelGradientUpdater::update_user_factors(RecDat* rec_dat,
    double gradient){
  model_->user_factors_.lin_combine(rec_dat->user,
      -gradient*learning_rate_*model_->user_vector_weight_,
      model_->item_factors_.get(rec_dat->item));
}
void SvdppModelGradientUpdater::update_item_factors(RecDat* rec_dat,
    double gradient){
  if (!model_->cache_is_valid(rec_dat)) model_->prediction(rec_dat);
  model_->item_factors_.lin_combine(rec_dat->item, -gradient*learning_rate_,
      &model_->cached_user_factor_);
}
void SvdppModelGradientUpdater::update_history_item_factors(RecDat* rec_dat,
    double gradient,vector<double>* item_vector){
  if (!model_->cache_is_valid(rec_dat)) model_->prediction(rec_dat);
  const vector<const RecDat*>* user_history =
      model_->user_history_container_.get_user_history(rec_dat->user);
  if(user_history != NULL){
    auto history_iterator = user_history->rbegin();
    auto cached_weight = model_->cached_weights_.begin();
    for(;
        history_iterator!=user_history->rend()
            && cached_weight!=model_->cached_weights_.end();
        history_iterator++,cached_weight++)
    {
      model_->history_item_factors_.lin_combine(
          (*history_iterator)->item,
          -gradient * learning_rate_ * (*cached_weight) * model_->cached_norm_
              * model_->history_weight_,
          item_vector); }
  }
}

void SvdppModelGradientUpdater::beginning_of_updating_cycle(){
  if(cumulative_item_updates_){
    Util::zero_out_vector(&cumulated_histvector_updates_);
    first_sample_ = NULL;
  }
}
void SvdppModelGradientUpdater::end_of_updating_cycle(){
  if(cumulative_item_updates_ && first_sample_!=NULL){
    update_history_item_factors(first_sample_,1,&cumulated_histvector_updates_);
    model_->cache_marked_invalid_ = true;
    first_sample_=NULL;
  }
}
