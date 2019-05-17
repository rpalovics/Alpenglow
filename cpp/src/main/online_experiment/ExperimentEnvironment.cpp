#include "ExperimentEnvironment.h"
#include "OnlineExperiment.h"


void ExperimentEnvironment::set_parameters(OnlineExperimentParameters* params){
  top_k_ = params->top_k;
  min_time_ = params->min_time;
  max_time_ = params->max_time;
  exclude_known_ = params->exclude_known;
  initialize_all_ = params->initialize_all;
  max_user_ = params->max_user;
  max_item_ = params->max_item;
  random_->set(params->random_seed);
}
void ExperimentEnvironment::update(RecDat* rec_dat){
  int item = rec_dat->item;
  int user = rec_dat->user;
  double score = rec_dat->score; 

  if (!GET_VECTORMAP(item_map_,item,false)){
    PUT_VECTORMAP(item_map_,item,true);
    items_.push_back(item);
  }
  if (!GET_VECTORMAP(user_map_,user,false)){
    PUT_VECTORMAP(user_map_,user,true);
    users_.push_back(user);
  }
  if(train_matrix_.get(user,item)==0){
    item_new_for_user_=true;
    train_matrix_.update(user,item,score);
  } else {
    item_new_for_user_=false;
  }
  popularity_sorted_container_.increase(item);
  popularity_container_.increase(item);
}
