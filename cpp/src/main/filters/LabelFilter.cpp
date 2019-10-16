#include "LabelFilter.h"

bool LabelFilter::active(RecDat* rec_dat){
  LABEL actual_label = label_container_.get_label(rec_dat->item);
  resize_last_labels(rec_dat->user);
  LABEL last_label_of_active_user = last_labels_[rec_dat->user];
  return (actual_label == last_label_of_active_user);
}

vector<int> LabelFilter::get_whitelist(int user){
  resize_last_labels(user);
  int last_label = last_labels_[user];
  vector<int> active_items = label_container_.get_entities(last_label);
  return active_items;
}

void LabelFilter::update(RecDat* rec_dat){
  LABEL label = label_container_.get_label(rec_dat->item);
  resize_last_labels(rec_dat->user);
  last_labels_[rec_dat->user]=label;
}

void LabelFilter::resize_last_labels(int user){
  if((int)last_labels_.size()<=user) last_labels_.resize(user+1,-1);
}
