#include "LabelFilter.h"

bool LabelFilter::active(RecDat* rec_dat){
  LABEL actual_label = label_container_.get_label(rec_dat->item);
  if((int)last_labels_.size()<=rec_dat->user) last_labels_.resize(rec_dat->user+1,-1);
  LABEL last_label_of_active_user = last_labels_[rec_dat->user];
  return (actual_label == last_label_of_active_user);
}

vector<pair<int,double>>* LabelFilter::get_personalized_items(int user){
  item_filter_.clear();
  if((int)last_labels_.size()<=user) last_labels_.resize(user+1,-1);
  LABEL last_label_of_active_user = last_labels_[user];
  if(last_label_of_active_user != -1){
    vector<ENTITY> items = label_container_.get_entities(last_label_of_active_user);
    for(ENTITY item : items){
      item_filter_.push_back(make_pair(item,-1));
    }
  }
  return &item_filter_;
}

void LabelFilter::update(RecDat* rec_dat){
  //called too early, delaying
  if(temp!=NULL){
    LABEL label = label_container_.get_label(temp->item);
    last_labels_[temp->user]=label;
  }
  temp = rec_dat;
}
