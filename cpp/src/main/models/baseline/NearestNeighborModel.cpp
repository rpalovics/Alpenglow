#include "NearestNeighborModel.h"

vector<pair<int,double>>* NearestNeighborModel::get_personalized_items(int user){
  if (user != active_user_) recompute_filter(user);
  return &active_user_filter_;
}

double NearestNeighborModel::prediction(RecDat* rec_dat){
  USER user = rec_dat->user;
  ITEM item = rec_dat->item;
  if (user != active_user_) recompute_filter(user);
  return GET_VECTORMAP(active_user_predictions_,item,0.0);
}

void NearestNeighborModel::recompute_filter(int user){
  active_user_filter_.clear();
  active_user_predictions_.clear();
  active_user_predictions_.resize(similarities_.size());

  auto user_history = user_histories_.get_user_history(user);
  if (user_history!=NULL){
    for(int user_history_index=(int)user_history->size()-1, weight_index=0;
        user_history_index>=0;
        user_history_index--,weight_index++){
      if(weights_[weight_index]<=gamma_threshold_) break;
      ITEM i = (*user_history)[user_history_index]->item;
      //vector<pair<ITEM,double>>& j_items = GET_VECTORMAP(similarities_,i,vector<pair<ITEM,double>>);
      vector<pair<ITEM,double>> empty_vector;
      vector<pair<ITEM,double>>& j_items = GET_VECTORMAP(similarities_,i,empty_vector);
      for(const auto& similarity_pair : j_items){
        ITEM j = similarity_pair.first;
        double similarity = similarity_pair.second;
        SCORE initial_pred = GET_VECTORMAP(active_user_predictions_,j,0.0);
        if(initial_pred==0){ active_user_filter_.push_back(make_pair(j,-1)); } //TODO ha weight*sim==0, hamis hozzaadas lehet
        ADD_VECTORMAP(active_user_predictions_,j,weights_[weight_index]*similarity);
        //cerr << "prediction i" << i << " j" << j << " s" << similarity << " w" << weights_[weight_index] << " p" << GET_VECTORMAP(active_user_predictions_,j,0) << endl;
      }
    }

    for(auto& filter_item : active_user_filter_){
      filter_item.second = active_user_predictions_[filter_item.first];
    }
    sort(
        active_user_filter_.begin(),
        active_user_filter_.end(),
        [](pair<int,double> a, pair<int,double> b) -> bool
           { return (a.second) > (b.second); }
        );
  }
  active_user_ = user;
}
