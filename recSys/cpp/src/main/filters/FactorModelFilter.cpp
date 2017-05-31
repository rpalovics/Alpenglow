#include "FactorModelFilter.h"


void FactorModelFilter::set_model(FactorModel* _model){
  model = _model;
  model->attach(this);
  user_factor_filter->set_factors(&model->user_factors_,&model->item_factors_);
  item_factor_filter->set_factors(&model->item_factors_,&model->user_factors_);
}

void FactorModelFilter::set_users(vector<int>* _users){
  users = _users;
  if(items!=NULL){
    user_factor_filter->set_entities(users,items);
    item_factor_filter->set_entities(items,users);
  }
}

void FactorModelFilter::set_items(vector<int>* _items){
  items = _items;
  if(users!=NULL){
    user_factor_filter->set_entities(users,items);
    item_factor_filter->set_entities(items,users);
  }
}
void FactorModelFilter::run(RecDat* rd){
  run(rd->time);
}
void FactorModelFilter::run(double time){ 
  user_factor_filter->run();
  item_factor_filter->run();
  compute_biases();
  compute_recencies(time);
  compute_sigmoids();
  sort(user_upper_bounds.begin(),user_upper_bounds.end(),sort_pair_descending_by_second<int>);
  sort(item_upper_bounds.begin(),item_upper_bounds.end(),sort_pair_descending_by_second<int>);
}
vector<pair<int,double>>* FactorModelFilter::get_global_users(){
  if (notified()) run(0.0); //TODO run: correct timestamp
  delete_notification();
  return &user_upper_bounds;
}
vector<pair<int,double>>* FactorModelFilter::get_global_items(){
  if (notified()) run(0.0); //TODO run: correct timestamp
  delete_notification();
  return &item_upper_bounds;
}

void FactorModelFilter::compute_biases(){
  if(model->use_user_bias_){
    compute_bias(&user_upper_bounds, model->user_bias_, users, &item_upper_bounds);
  }
  if(model->use_item_bias_){
    compute_bias(&item_upper_bounds, model->item_bias_, items, &user_upper_bounds);
  }
}

void FactorModelFilter::compute_bias(vector<pair<int,double> >* bounds, Bias& biases, vector<int>* entities, vector<pair<int,double> >* other_bounds){
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
  if(model->user_recency_!=NULL){
    compute_recency(&user_upper_bounds,model->user_recency_,time);
  }
  if(model->item_recency_!=NULL){
    compute_recency(&item_upper_bounds,model->item_recency_,time);
  }
}

void FactorModelFilter::compute_recency(vector<pair<int,double> >* bounds, Recency* recency, double time){
    for(uint ii=0;ii<bounds->size();ii++){
      bounds->at(ii).second*=recency->get(bounds->at(ii).first,time);
    }
}

void FactorModelFilter::compute_sigmoids(){
  if(model->use_sigmoid_){
    compute_sigmoid(&user_upper_bounds);
    compute_sigmoid(&item_upper_bounds);
  } 
}
void FactorModelFilter::compute_sigmoid(vector<pair<int,double> >* bounds){
  for(uint ii=0;ii<bounds->size();ii++){
    bounds->at(ii).second=Util::sigmoid_function(bounds->at(ii).second);
  }
  
}

void FactorFilter::set_factors(Factors * _factors, Factors * _other_factors){
  factors = _factors;
  other_factors = _other_factors;
}

void FactorFilter::set_entities(vector <int> * _entities, vector <int> * _other_entities){
  entities = _entities;
  other_entities = _other_entities;
}

void FactorFilter::compute_bound_vectors(){
  for(uint ii=0; ii<other_entities->size(); ii++){
    int entity = (*other_entities)[ii];
    vector <double> * factor =  other_factors->get(entity);
    if(ii==0) init_bounds(factor);
    else analyze_bounds(factor);
  }
}

void FactorFilter::init_bounds(vector <double> * factor){
  upper = *factor;
  lower = *factor;
}

void FactorFilter::analyze_bounds(vector <double> * factor){
  for(uint kk=0; kk<factor->size(); kk++){
    if((*factor)[kk] > upper[kk]) upper[kk] = (*factor)[kk];
    if((*factor)[kk] < lower[kk]) lower[kk] = (*factor)[kk];
  }
}

void FactorFilter::compute_upper_bounds(){
  upper_bounds->clear();
  upper_bounds->reserve(entities->size());
  for(uint ii = 0; ii<entities->size(); ii++){
    int entity = (*entities)[ii];
    compute_upper_bound(entity);
  }
}

void FactorFilter::compute_upper_bound(int entity){
  vector <double> * factor = factors->get(entity);
  double sum = 0;
  for(uint kk=0; kk<factor->size(); kk++){
    if((*factor)[kk] > 0 and upper[kk] > 0) sum+= (*factor)[kk] * upper[kk];
    if((*factor)[kk] < 0 and lower[kk] < 0) sum+= (*factor)[kk] * lower[kk];
  }
  upper_bounds->push_back(make_pair(entity,sum));
}

void FactorFilter::run(){
  compute_bound_vectors();
  compute_upper_bounds();
}

