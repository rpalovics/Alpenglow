#include "NearestNeighborModelUpdater.h"

void NearestNeighborModelUpdater::message(UpdaterMessage message){
  if (message == UpdaterMessage::end_of_offline_update){
    model_->active_user_=-1;
    recompute_similarity();
  }
}

void NearestNeighborModelUpdater::update(RecDat* rec_dat){
  model_->active_user_=-1;
  model_-> user_histories_.update(rec_dat);
  auto user_history = model_->user_histories_.get_user_history(rec_dat->user);
  int pos = user_history->size()-1;
  for(uint weight_size = model_->weights_.size();weight_size<user_history->size();weight_size++){
    model_->weights_.push_back(model_->weights_[weight_size-1]*model_->gamma_);
  }
  PUT_VECVECMAP(userlist_for_items_,rec_dat->item,make_pair(rec_dat->user,pos));

  if(period_mode_!="off"){
    int period = rec_dat->time / compute_similarity_period_; //TODO: samplenum mode
    if ( last_similarity_recompute_period_ != period ){
      recompute_similarity();
      last_similarity_recompute_period_ = period;
    }
  }
}

void NearestNeighborModelUpdater::recompute_similarity(){
  model_->similarities_.clear();
  for(ITEM item = 0; item<(int)userlist_for_items_.size() ; item++){
    vector<double> similarity_sums(userlist_for_items_.size());
    vector<pair<ITEM,double>> item_filter;
    const auto& user_list = userlist_for_items_[item];
    for(auto user_pos_pair : user_list){
      auto user_hist = model_->user_histories_.get_user_history(user_pos_pair.first);
      for(uint user_history_index=user_pos_pair.second+1, weight_index=0;
          user_history_index<user_hist->size();
          user_history_index++,weight_index++){
        ITEM j = (*user_hist)[user_history_index]->item;
        if(similarity_sums[j]==0) item_filter.push_back(make_pair(j,0));
        similarity_sums[j]+=model_->weights_[weight_index];
      }
      if(model_->direction_=="both"){
        for(int user_history_index=user_pos_pair.second-1, weight_index=0;
            user_history_index>=0;
            user_history_index--,weight_index++){
          ITEM j = (*user_hist)[user_history_index]->item;
          if(similarity_sums[j]==0) item_filter.push_back(make_pair(j,0));
          similarity_sums[j]+=model_->weights_[weight_index];
        }
      }
    }
    double norm=0; 
    if (model_->norm_type_=="off") norm=1.0;
    else /*if (model_->norm_type_=="num")*/ norm = 1.0/user_list.size();
    
    for(auto& item_sim_pair:item_filter){
      item_sim_pair.second=similarity_sums[item_sim_pair.first]*norm;
    }
    sort(
        item_filter.begin(),
        item_filter.end(),
        [](pair<int,double> a, pair<int,double> b) -> bool
           { return (a.second) > (b.second); }
        );
    for(int i=0;i<(int)item_filter.size() and i<model_->num_of_neighbors_;i++){
      PUT_VECVECMAP(model_->similarities_,item,item_filter[i]);
    }
  }
}
