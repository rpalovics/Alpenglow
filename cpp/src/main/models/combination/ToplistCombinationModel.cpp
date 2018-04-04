#include "ToplistCombinationModel.h"

void ToplistCombinationModel::add(RecDat* rec_dat){
  for(auto model:models_){
    model->add(rec_dat);
  }
}
double ToplistCombinationModel::prediction(RecDat* rec_dat){
  if(rec_dat->id == last_id_ and rec_dat->user==last_user_ and rec_dat->time ==last_timestamp_){
    compute_score_map();
    return scores_[rec_dat->item]; //TODO return 0 if item is not in the map
  }
  generate_random_values_for_toplists();
  compute_last_occ_of_models();
  if (!test_top_k()) return 0; //toplist should be cleared? Not exactly.
  compute_toplists();
  merge_toplists();
  last_id_ = rec_dat->id;
  last_user_ = rec_dat->user;
  last_timestamp_ = rec_dat->time;
  return 0; //TODO find index of the item, return 1/(index+1)
}
void ToplistCombinationModel::write(ostream& file){
  for(auto model:models_){
    model->write(file);
  }
}
void ToplistCombinationModel::read(istream& file){
  for(auto model:models_){
    model->read(file);
  }
}

RankingScoreIterator* ToplistCombinationModel::get_ranking_score_iterator(int user){
  if(user!=last_user_) throw exception(); //prediction should be called first, the parameter here is user, not recdat, we can't update state fields properly
  return NULL; //TODO
}

void ToplistCombinationModel::generate_random_values_for_toplists(){
  random_model_indices_.clear();
  for(int i=0;i<experiment_environment_->get_top_k();i++){
    random_model_indices_.push_back(random_->get_discrete(distribution_));
  }
}
void ToplistCombinationModel::compute_score_map(){}
void ToplistCombinationModel::compute_last_occ_of_models(){}
bool ToplistCombinationModel::test_top_k(){return false;}
void ToplistCombinationModel::compute_toplists(){}
void ToplistCombinationModel::merge_toplists(){}
