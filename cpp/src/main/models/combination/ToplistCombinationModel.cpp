#include "ToplistCombinationModel.h"

void ToplistCombinationModel::add(RecDat* rec_dat){
  for(auto model:wms_.models_){
    model->add(rec_dat);
  }
}
double ToplistCombinationModel::prediction(RecDat* rec_dat){
  if(rec_dat->id != last_id_ or rec_dat->user!=last_user_ or rec_dat->time !=last_timestamp_){
    scores_.clear();
    recompute_predictions(rec_dat);
  }
  if(scores_.find(rec_dat->item)==scores_.end()) return 0;
  else return scores_[rec_dat->item];
}
void ToplistCombinationModel::recompute_predictions(RecDat* rec_dat){
  if(!random_values_generated_){
    generate_random_values_for_toplists();
    compute_last_occ_of_models();
    random_values_generated_ = true;
  }
  if (!test_top_k(rec_dat)) return;
  compute_toplists(rec_dat);
  merge_toplists();
  compute_score_map();
  last_id_ = rec_dat->id;
  last_user_ = rec_dat->user;
  last_timestamp_ = rec_dat->time;
  random_values_generated_ = false;
}
void ToplistCombinationModel::write(ostream& file){
  for(auto model:wms_.models_){
    model->write(file);
  }
}
void ToplistCombinationModel::read(istream& file){
  for(auto model:wms_.models_){
    model->read(file);
  }
}

void ToplistCombinationModel::generate_random_values_for_toplists(){
  random_model_indices_.clear();
  for(int i=0;i<experiment_environment_->get_top_k();i++){
    random_model_indices_.push_back(random_.get_discrete(wms_.distribution_));
  }
}
void ToplistCombinationModel::compute_score_map(){
  for(uint i=0;i<toplist_.size();i++){
    int item = toplist_[i].first;
    double score = 1.0/(i+1);
    scores_.insert(make_pair(item,score));
  }
}
void ToplistCombinationModel::compute_last_occ_of_models(){
  last_occ_of_models_.clear();
  last_occ_of_models_.resize(wms_.models_.size(),-1);
  for(uint i=0;i<random_model_indices_.size();i++){
    int model = random_model_indices_[i];
    last_occ_of_models_[model]=i;
  }
}
bool ToplistCombinationModel::test_top_k(RecDat* rec_dat){
  for(uint i=0;i<wms_.models_.size();i++){
    int rank = rank_computers_[i]->get_rank(rec_dat);
    if(rank<=last_occ_of_models_[i]) return true;
  }
  return false;
}
void ToplistCombinationModel::compute_toplists(RecDat* rec_dat){
  toplists_.clear();
  toplists_.resize(wms_.models_.size());
  for(uint i=0;i<toplists_.size();i++){
    toplists_[i].resize(last_occ_of_models_[i]+1);
  }
  for(uint i=0;i<toplist_creators_.size();i++){
    vector<RecDat>* toplist = toplist_creators_[i]->run(rec_dat);
    for(uint j=0;j<toplists_[i].size();j++){
      toplists_[i][j].first = toplist->at(j).item;
      toplists_[i][j].second = toplist->at(j).score;
    }
  }
}
void ToplistCombinationModel::merge_toplists(){
  toplist_.clear();
  vector<int> model_counters(wms_.models_.size());
  set<int> used_items;
  for(uint i=0;i<random_model_indices_.size();i++){
    int active_model = random_model_indices_[i];
    int model_counter = model_counters[active_model]; //skip used items
    while(model_counters[active_model]<(int)toplists_[active_model].size()){
      int possible_item = toplists_[active_model][model_counter].first;
      if(used_items.find(possible_item)==used_items.end()) break;
      model_counter++;
    }
    auto toplist_element = toplists_[active_model][model_counter];
    used_items.insert(toplist_element.first);
    toplist_.push_back(toplist_element);
    model_counters[active_model] = model_counter;
    model_counters[active_model]++;
  }
}

RankingScoreIterator* ToplistCombinationModel::get_ranking_score_iterator(int user){
  if(random_values_generated_) return NULL; //prediction was called, but toplist is not generated
  if(user!=last_user_) throw runtime_error("prediction should be called first, the parameter here is user, not recdat, we can't update state fields properly");
  rsi_.set_up(toplist_);
  return &rsi_;
}

bool ToplistCombinationModelRankingScoreIterator::has_next(double bound){
  return ( (int)current_scores_.size() > counter_ &&
           current_scores_[counter_].second >= bound );
}
pair<int, double> ToplistCombinationModelRankingScoreIterator::get_next(){
  return current_scores_.at(counter_++);
}
