#include "DummyModelFilter.h"

void DummyModelFilter::run(RecDat*){
  if(last_items_size_ != (int)items_->size()){
    item_filter_.clear();
    for(auto item_it = items_->begin();item_it!=items_->end();item_it++){
      item_filter_.push_back(make_pair(*item_it,-1));
    }
    last_items_size_=items_->size();
  }
  if(last_users_size_ != (int)users_->size()){
    user_filter_.clear();
    for(auto user_it = users_->begin();user_it!=users_->end();user_it++){
      user_filter_.push_back(make_pair(*user_it,-1));
    }
    last_users_size_=users_->size();
  }
}

vector<pair<int,double>>* DummyModelFilter::get_global_users(){
  return &user_filter_;
}

vector<pair<int,double>>* DummyModelFilter::get_global_items(){
  return &item_filter_;
}

