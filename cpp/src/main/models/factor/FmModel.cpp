#include "FmModel.h"
#include "../../utils/Util.h"

void FmModel::add(RecDat* rec_dat){
  if((int)seen_users_.size() <= rec_dat->user){
    seen_users_.resize(rec_dat->user+1, false);
  }
  if((int)seen_items_.size() <= rec_dat->item){
    seen_items_.resize(rec_dat->item+1, false);
  }
  if(!seen_users_[rec_dat->user] && user_attributes_==NULL){
    user_factors_.init(rec_dat->user);
  }
  if(!seen_items_[rec_dat->item] && item_attributes_==NULL){
    item_factors_.init(rec_dat->item);
  }
  seen_users_[rec_dat->user] = true;
  seen_items_[rec_dat->item] = true;
}

bool FmModel::can_predict_(RecDat* rec_dat){
  if((int)seen_items_.size() <= rec_dat->item || !seen_items_[rec_dat->item]){
    return false;
  }
  if((int)seen_users_.size() <= rec_dat->user || !seen_users_[rec_dat->user]){
    return false;
  }
  return true;
}

double FmModel::prediction(RecDat* rec_dat){
  if(!can_predict_(rec_dat)){
    return 0;
  }
  vector<double> v1 = sum_attribute_vector(user_factors_, get_attributes(user_attributes_, rec_dat->user));
  vector<double> v2 = sum_attribute_vector(item_factors_, get_attributes(item_attributes_, rec_dat->item));
  return Util::scalar_product(&v1, &v2);
}

void FmModel::clear(){
  user_factors_.clear();
  item_factors_.clear();
  seen_items_.clear();
  seen_users_.clear();
  if(user_attributes_ != NULL){
    for(int i = user_attributes_->get_max_attribute_index(); i!=-1; i--){
      user_factors_.init_rand(i);
    }
  }
  if(item_attributes_ != NULL){
    for(int i = item_attributes_->get_max_attribute_index(); i!=-1; i--){
      item_factors_.init_rand(i);
    }
  }
}


const unordered_map<int, double>& FmModel::get_attributes(SparseAttributeContainer *container, int id) {
  if(container == NULL){
    indentity_ = unordered_map<int, double>({{id,1}});
    return indentity_;
  } else {
    return container->get_attributes(id);
  }
}

vector<double> FmModel::sum_attribute_vector(
  Factors &factors,
  const unordered_map<int, double> &attributes
){
  vector<double> tmp(dimension_, 0);
  for(const auto &i_v : attributes){
    vector<double> *latent = factors.get(i_v.first);
    if(latent != NULL){
      for(uint i=0; i < tmp.size(); i++){
        tmp[i] += i_v.second * (*latent)[i];
      }
    }
  }
  return tmp;
}

void FmModel::update_attributes(
  Factors &factors,
  const unordered_map<int, double> &attributes,
  vector<double> &gradient_direction,
  double gradient_multiplier
){
  for(const auto &i_v : attributes){
    factors.add(i_v.first, &gradient_direction, i_v.second*gradient_multiplier);
  }
}

bool FmModel::autocalled_initialize() {
  clear();
  return true;
}
