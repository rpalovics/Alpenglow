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

//ez hatekonysagnovelo filter, ezert az active fuggvenyt nem kell megirni
vector<pair<int,double>>* TransitionProbabilityModel::get_personalized_items(int user){
  filter_.clear();
  auto actual_frequency_map = get_frequency_map(user);
  if(actual_frequency_map==NULL) return &filter_;//empty list

  for(auto frequency = actual_frequency_map->begin();frequency!=actual_frequency_map->end();frequency++){
    filter_.push_back(make_pair(frequency->first,log(frequency->second+1)));
  }
  sort(
      filter_.begin(),
      filter_.end(),
      [](pair<int,double> a, pair<int,double> b) -> bool
         { return (a.second) > (b.second); }
      );
  return &filter_;
}

map<int,int>* TransitionProbabilityModel::get_frequency_map(int user){
  if((int)last_entities_.size()<=user or last_entities_[user]==-1) return NULL; //no last item
  int last_item = last_entities_[user];
  if((int)transition_frequencies_.size()<=last_item) return NULL;
  return &transition_frequencies_[last_item];
}

//int TransitionProbabilityModel::get_frequency_sum(int user){
//  if((int)last_entities_.size()<=user or last_entities_[user]==-1) return 0; //no last item
//  int last_item = last_entities_[user];
//  if((int)frequency_sums_.size()<=last_item) return NULL;
//  return frequency_sums_[last_item];
//}
