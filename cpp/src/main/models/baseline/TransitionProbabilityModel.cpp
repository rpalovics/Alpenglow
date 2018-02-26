#include "TransitionProbabilityModel.h"

double TransitionProbabilityModel::prediction(RecDat* rec_dat){
  int user = rec_dat->user;
  auto actual_frequency_map = get_frequency_map(user);
  if(actual_frequency_map==NULL) return 0;
  int item = rec_dat->item;
  auto frequency = actual_frequency_map->find(item);
  if(frequency!=actual_frequency_map->end()){
    return log(frequency->second+1);
  } else {
    return 0;
  }
}

map<int,int>* TransitionProbabilityModel::get_frequency_map(int user){
  if((int)lastly_visited_entities_.size()<=user or lastly_visited_entities_[user]==-1) return NULL; //no last item or last label
  int last_item = lastly_visited_entities_[user];
  if((int)transition_frequencies_.size()<=last_item) return NULL;
  return &transition_frequencies_[last_item];
}

RankingScoreIterator* TransitionProbabilityModel::get_ranking_score_iterator(int user){
  if(user==active_user_){
    ranking_score_iterator_.reinit();
  } else {
    auto actual_frequency_map = get_frequency_map(user);
    ranking_score_iterator_.set_up(actual_frequency_map);
    active_user_=user;
  }
  return &ranking_score_iterator_;
}

void TransitionProbabilityModelRankingScoreIterator::set_up(map<int,int>* actual_frequency_map){
  clear();
  if(actual_frequency_map==NULL) return; //all predictions will be 0

  for(auto frequency = actual_frequency_map->begin();frequency!=actual_frequency_map->end();frequency++){
    current_scores_.push_back(make_pair(frequency->first,log(frequency->second+1)));
  }
  sort(
      //instead of sorting here we could keep transition lists always sorted
      //RSI could be implemented without sorting at all
      //but here sort is fast enough as the list to be sorted is short
      current_scores_.begin(),
      current_scores_.end(),
      [](pair<int,double> a, pair<int,double> b) -> bool
         { return (a.second) > (b.second); }
      );
}
bool TransitionProbabilityModelRankingScoreIterator::has_next(double bound){
  return ( (int)current_scores_.size() > counter_ &&
           current_scores_[counter_].second >= bound );
}
pair<int, double> TransitionProbabilityModelRankingScoreIterator::get_next(){
  return current_scores_.at(counter_++);
}



//int TransitionProbabilityModel::get_frequency_sum(int user){
//  if((int)lastly_visited_entities_.size()<=user or lastly_visited_entities_[user]==-1) return 0; //no last item
//  int last_item = lastly_visited_entities_[user];
//  if((int)frequency_sums_.size()<=last_item) return NULL;
//  return frequency_sums_[last_item];
//}
