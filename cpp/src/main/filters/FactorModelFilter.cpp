#include "FactorModelFilter.h"


void FactorModelFilter::set_model(FactorModel* model){
  model_ = model;
  user_factor_filter_.set_factors(&model_->user_factors_,&model_->item_factors_);
  item_factor_filter_.set_factors(&model_->item_factors_,&model_->user_factors_);
}

void FactorModelFilter::set_users(const vector<int>* users){
  users_ = users;
  if(items_!=NULL){
    user_factor_filter_.set_entities(users_,items_);
    item_factor_filter_.set_entities(items_,users_);
  }
}

void FactorModelFilter::set_items(const vector<int>* items){
  items_ = items;
  if(users_!=NULL){
    user_factor_filter_.set_entities(users_,items_);
    item_factor_filter_.set_entities(items_,users_);
  }
}
void FactorModelFilter::run(RecDat* rd){
  double time=rd->time;
  user_factor_filter_.run();
  item_factor_filter_.run();
  compute_biases();
  compute_recencies(time);
  compute_sigmoids();
  sort(user_upper_bounds_.begin(),user_upper_bounds_.end(),
      [](pair<int,double> a, pair<int,double> b) -> bool { return (a.second) > (b.second); });
  sort(item_upper_bounds_.begin(),item_upper_bounds_.end(),
      [](pair<int,double> a, pair<int,double> b) -> bool { return (a.second) > (b.second); });
}

void FactorModelFilter::compute_biases(){
  if(model_->use_user_bias_){
    compute_bias(&user_upper_bounds_, model_->user_bias_, users_, &item_upper_bounds_);
  }
  if(model_->use_item_bias_){
    compute_bias(&item_upper_bounds_, model_->item_bias_, items_, &user_upper_bounds_);
  }
}

void FactorModelFilter::compute_bias(vector<pair<int,double> >* bounds, Bias& biases, const vector<int>* entities, vector<pair<int,double> >* other_bounds){
    for(uint ii=0;ii<bounds->size();ii++){
      bounds->at(ii).second+=biases.get(bounds->at(ii).first);
    }
    double max_bias = biases.get(entities->at(0));
    for(uint ii=0;ii<entities->size();ii++){
      int entity = entities->at(ii);
      double bias = biases.get(entity);
      if(max_bias<bias){max_bias=bias;}
    }
    for(uint ii=0;ii<other_bounds->size();ii++){
      other_bounds->at(ii).second+=max_bias;
    }
}

void FactorModelFilter::compute_recencies(double time){
  if(model_->user_recency_!=NULL){
    compute_recency(&user_upper_bounds_,model_->user_recency_,time);
  }
  if(model_->item_recency_!=NULL){
    compute_recency(&item_upper_bounds_,model_->item_recency_,time);
  }
}

void FactorModelFilter::compute_recency(vector<pair<int,double> >* bounds, Recency* recency, double time){
    for(uint ii=0;ii<bounds->size();ii++){
      bounds->at(ii).second*=recency->get(bounds->at(ii).first,time);
    }
}

void FactorModelFilter::compute_sigmoids(){
  if(model_->use_sigmoid_){
    compute_sigmoid(&user_upper_bounds_);
    compute_sigmoid(&item_upper_bounds_);
  } 
}
void FactorModelFilter::compute_sigmoid(vector<pair<int,double> >* bounds){
  for(uint ii=0;ii<bounds->size();ii++){
    bounds->at(ii).second=Util::sigmoid_function(bounds->at(ii).second);
  }
  
}

void FactorFilter::set_factors(Factors* factors, Factors* other_factors){
  factors_ = factors;
  other_factors_ = other_factors;
}

void FactorFilter::set_entities(const vector<int>* entities, const vector<int>* other_entities){
  entities_ = entities;
  other_entities_ = other_entities;
}

void FactorFilter::compute_bound_vectors(){
  for(uint ii=0; ii<other_entities_->size(); ii++){
    int entity = (*other_entities_)[ii];
    vector<double>* factor =  other_factors_->get(entity);
    if(ii==0) init_bounds(factor);
    else analyze_bounds(factor);
  }
}

void FactorFilter::init_bounds(vector <double> * factor){
  upper_ = *factor;
  lower_ = *factor;
}

void FactorFilter::analyze_bounds(vector <double> * factor){
  for(uint kk=0; kk<factor->size(); kk++){
    if((*factor)[kk] > upper_[kk]) upper_[kk] = (*factor)[kk];
    if((*factor)[kk] < lower_[kk]) lower_[kk] = (*factor)[kk];
  }
}

void FactorFilter::compute_upper_bounds(){
  upper_bounds_->clear();
  upper_bounds_->reserve(entities_->size());
  for(uint ii = 0; ii<entities_->size(); ii++){
    int entity = (*entities_)[ii];
    compute_upper_bound(entity);
  }
}

void FactorFilter::compute_upper_bound(int entity){
  vector<double>* factor = factors_->get(entity);
  double sum = 0;
  for(uint kk=0; kk<factor->size(); kk++){
    if((*factor)[kk] > 0 and upper_[kk] > 0) sum+= (*factor)[kk] * upper_[kk];
    if((*factor)[kk] < 0 and lower_[kk] < 0) sum+= (*factor)[kk] * lower_[kk];
  }
  upper_bounds_->push_back(make_pair(entity,sum));
}

void FactorFilter::run(){
  compute_bound_vectors();
  compute_upper_bounds();
}

