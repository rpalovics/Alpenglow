#include "FactorModelFilter.h"


void FactorModelFilter::set_model(FactorModel * _model){
  model = _model;
  userFactorFilter->setFactors(&model->user_factors_,&model->item_factors_);
  itemFactorFilter->setFactors(&model->item_factors_,&model->user_factors_);
}

void FactorModelFilter::set_users(vector<int>* _users){
  users = _users;
  if(items!=NULL){
    userFactorFilter->setEntities(users,items);
    itemFactorFilter->setEntities(items,users);
  }
}

void FactorModelFilter::set_items(vector<int>* _items){
  items = _items;
  if(users!=NULL){
    userFactorFilter->setEntities(users,items);
    itemFactorFilter->setEntities(items,users);
  }
}
void FactorModelFilter::run(RecDat* rd){
  run(rd->time);
}
void FactorModelFilter::run(double time){ 
  userFactorFilter->run();
  itemFactorFilter->run();
  computeBiases();
  computeRecencies(time);
  computeSigmoids();
  sort(userUpperBounds.begin(),userUpperBounds.end(),sortPairDescendingBySecond<int>);
  sort(itemUpperBounds.begin(),itemUpperBounds.end(),sortPairDescendingBySecond<int>);
}

void FactorModelFilter::computeBiases(){
  if(model->use_user_bias_){
    computeBias(&userUpperBounds, model->user_bias_, users, &itemUpperBounds);
  }
  if(model->use_item_bias_){
    computeBias(&itemUpperBounds, model->item_bias_, items, &userUpperBounds);
  }
}

void FactorModelFilter::computeBias(vector<pair<int,double> >* bounds, Bias& biases, vector<int>* entities, vector<pair<int,double> >* otherBounds){
    for(uint ii=0;ii<bounds->size();ii++){
      bounds->at(ii).second+=biases.get(bounds->at(ii).first);
    }
    double maxBias = biases.get(entities->at(0));
    for(uint ii=0;ii<entities->size();ii++){
      int entity = entities->at(ii);
      double bias = biases.get(entity);
      if(maxBias<bias){maxBias=bias;}
    }
    for(uint ii=0;ii<otherBounds->size();ii++){
      otherBounds->at(ii).second+=maxBias;
    }
}

void FactorModelFilter::computeRecencies(double time){
  if(model->user_recency_!=NULL){
    computeRecency(&userUpperBounds,model->user_recency_,time);
  }
  if(model->item_recency_!=NULL){
    computeRecency(&itemUpperBounds,model->item_recency_,time);
  }
}

void FactorModelFilter::computeRecency(vector<pair<int,double> >* bounds, Recency* recency, double time){
    for(uint ii=0;ii<bounds->size();ii++){
      bounds->at(ii).second*=recency->get(bounds->at(ii).first,time);
    }
}

void FactorModelFilter::computeSigmoids(){
  if(model->use_sigmoid_){
    computeSigmoid(&userUpperBounds);
    computeSigmoid(&itemUpperBounds);
  } 
}
void FactorModelFilter::computeSigmoid(vector<pair<int,double> >* bounds){
  for(uint ii=0;ii<bounds->size();ii++){
    bounds->at(ii).second=Util::sigmoid_function(bounds->at(ii).second);
  }
  
}

void FactorFilter::setFactors(Factors * _factors, Factors * _otherFactors){
  factors = _factors;
  otherFactors = _otherFactors;
}

void FactorFilter::setEntities(vector <int> * _entities, vector <int> * _otherEntities){
  entities = _entities;
  otherEntities = _otherEntities;
}

void FactorFilter::computeBoundVectors(){
  for(uint ii=0; ii<otherEntities->size(); ii++){
    int entity = (*otherEntities)[ii];
    vector <double> * factor =  otherFactors->get(entity);
    if(ii==0) initBounds(factor);
    else analyzeBounds(factor);
  }
}

void FactorFilter::initBounds(vector <double> * factor){
  upper = *factor;
  lower = *factor;
}

void FactorFilter::analyzeBounds(vector <double> * factor){
  for(uint kk=0; kk<factor->size(); kk++){
    if((*factor)[kk] > upper[kk]) upper[kk] = (*factor)[kk];
    if((*factor)[kk] < lower[kk]) lower[kk] = (*factor)[kk];
  }
}

void FactorFilter::computeUpperBounds(){
  upperBounds->clear();
  upperBounds->reserve(entities->size());
  for(uint ii = 0; ii<entities->size(); ii++){
    int entity = (*entities)[ii];
    computeUpperBound(entity);
  }
}

void FactorFilter::computeUpperBound(int entity){
  vector <double> * factor = factors->get(entity);
  double sum = 0;
  for(uint kk=0; kk<factor->size(); kk++){
    if((*factor)[kk] > 0 and upper[kk] > 0) sum+= (*factor)[kk] * upper[kk];
    if((*factor)[kk] < 0 and lower[kk] < 0) sum+= (*factor)[kk] * lower[kk];
  }
  upperBounds->push_back(make_pair(entity,sum));
}

void FactorFilter::run(){
  computeBoundVectors();
  computeUpperBounds();
}

