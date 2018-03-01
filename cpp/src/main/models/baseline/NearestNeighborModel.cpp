#include "NearestNeighborModel.h"

double NearestNeighborModel::prediction(RecDat* rec_dat){
  USER user = rec_dat->user;
  ITEM item = rec_dat->item;
  if (user != active_user_) recompute(user);
  return GET_VECTORMAP(active_user_predictions_,item,0.0);
}

RankingScoreIterator* NearestNeighborModel::get_ranking_score_iterator(int user){
  if(user==active_user_){
    ranking_score_iterator_.reinit();
  } else {
    recompute(user);
  }
  return &ranking_score_iterator_;
}
void NearestNeighborModel::recompute(int user){
  active_user_predictions_.clear();
  active_user_predictions_.resize(similarities_.size());
  nonzero_items_.clear();

  recompute_prediction(user);
  ranking_score_iterator_.set_up(active_user_predictions_, nonzero_items_);
  active_user_ = user;
}

void NearestNeighborModel::recompute_prediction(int user){
  auto user_history = user_histories_.get_user_history(user);
  if (user_history==NULL) return;
  for(int user_history_index=(int)user_history->size()-1, weight_index=0;
      user_history_index>=0;
      user_history_index--,weight_index++){
    if(weights_[weight_index]<=gamma_threshold_) break;
    ITEM visited_item = (*user_history)[user_history_index]->item;
    vector<pair<ITEM,double>> empty_vector;
    vector<pair<ITEM,double>>& similar_items = GET_VECTORMAP(similarities_,visited_item,empty_vector);
    for(const auto& similarity_pair : similar_items){
      ITEM j = similarity_pair.first;
      double similarity = similarity_pair.second;
      SCORE initial_pred = GET_VECTORMAP(active_user_predictions_,j,0.0);
      if(initial_pred==0){ nonzero_items_.push_back(j); }
      ADD_VECTORMAP(active_user_predictions_,j,weights_[weight_index]*similarity);
    }
  }
  
}

void NearestNeighborModelRankingScoreIterator::set_up(vector<SCORE>& predictions, vector<ITEM>& active_items){
  clear();
  for(auto& item : active_items){
    double prediction = GET_VECTORMAP(predictions,item,0.0);
    current_scores_.push_back(make_pair(item,prediction));
  }
  sort(
      current_scores_.begin(),
      current_scores_.end(),
      [](pair<int,double> a, pair<int,double> b) -> bool
         { return (a.second) > (b.second); }
      );
}

bool NearestNeighborModelRankingScoreIterator::has_next(double bound){
  return ( (int)current_scores_.size() > counter_ &&
           current_scores_[counter_].second >= bound );
}
pair<int, double> NearestNeighborModelRankingScoreIterator::get_next(){
  return current_scores_.at(counter_++);
}
