#include "OnlineDataUpdater.h"

void OnlineDataUpdater::setParameters(OnlineDataUpdaterParameters* parameters){
  train_matrix_ = parameters->trainMatrix;
  items_ = parameters->items;
  users_ = parameters->users;
  popularity_sorted_container_ = parameters->pop;
  popularity_container_ = parameters->popContainer;
}

void OnlineDataUpdater::update(RecDat* rec_dat){
  int item = rec_dat->item;
  int user = rec_dat->user;
  double score = rec_dat->score; 

  if(items_ != NULL){
    if((int)item_map_.size()<=item) item_map_.resize(item+1,false);
    if(!item_map_[item]){
      item_map_[item]=true;
      items_->push_back(item);
    }
  }
  if(users_ != NULL){
    if((int)user_map_.size()<=user) user_map_.resize(user+1,false);
    if(!user_map_[user]){
      user_map_[user]=true;
      users_->push_back(user);
    }
  }
  if(train_matrix_ != NULL){
    if(train_matrix_->get(user,item)==0){
      train_matrix_->update(user,item,score);
    }
  }
  if(popularity_sorted_container_!=NULL){
    popularity_sorted_container_->increase(item);
  }
  if(popularity_container_!=NULL){
    popularity_container_->increase(item);
  }
}

