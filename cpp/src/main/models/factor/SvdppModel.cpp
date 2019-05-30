#include "SvdppModel.h"
#include <iostream>
#include <fstream>


void SvdppModel::set_parameters(SvdppModelParameters* parameters){
  factors_parameters.begin_min=parameters->begin_min;
  factors_parameters.begin_max=parameters->begin_max;
  factors_parameters.dimension=parameters->dimension;
  factors_parameters.seed=parameters->seed;
  user_factors_.set_parameters(factors_parameters);
  factors_parameters.seed+=1;
  item_factors_.set_parameters(factors_parameters);
  factors_parameters.seed+=1;
  history_item_factors_.set_parameters(factors_parameters);
}

void SvdppModel::clear(){
  last_user_=-1;
  last_time_=-1;
  last_id_=-1;
  invalidate_user_factor_=true;

  user_factors_.clear();
  history_item_factors_.clear();
  item_factors_.clear();
  user_history_container_.clear_all();
  if(initialize_all_){
    for(int user=0;user<=max_user_;user++) user_factors_.init(user);
    for(int item=0;item<=max_item_;item++){
      item_factors_.init(item);
      history_item_factors_.init(item);
    }
  }
}

void SvdppModel::add(RecDat *rec_dat){
  user_factors_.init(rec_dat->user);
  item_factors_.init(rec_dat->item);
  history_item_factors_.init(rec_dat->item);
}

double SvdppModel::prediction(RecDat *rec_dat){
  compute_user_factor(rec_dat);
  double val = Util::scalar_product(&cached_user_factor_,item_factors_.get(rec_dat->item));
  if(use_sigmoid_)
    return Util::sigmoid_function(val);
  else return val;
}

void SvdppModel::compute_user_factor(RecDat* rec_dat){
  if(cache_is_valid(rec_dat)) return; //do not recompute user vector during evaluation
  auto user_history = user_history_container_.get_user_history(rec_dat->user);
  if(user_history!=NULL && user_history->size()!=0){ //TODO assert: should not happen that uh size == 0
    cached_user_factor_ = compute_histvector_sum(rec_dat,user_history); 
    double norm = compute_norm(user_history->size());
    Util::multiply_vector(norm*history_weight_,&cached_user_factor_);
  } else {
    Util::zero_out_vector(&cached_user_factor_);
  }
  vector<double>* user_vector=user_factors_.get(rec_dat->user);
  Util::sum_update_with(&cached_user_factor_,user_vector,user_vector_weight_);
}

bool SvdppModel::cache_is_valid(RecDat* rec_dat){
  //assumption: if we test cache, and it is not valid, we will immediately recompute it
  if(!invalidate_user_factor_ && last_user_==rec_dat->user && last_time_==rec_dat->time && last_id_==rec_dat->id){
    return true;
  } else {
    invalidate_user_factor_=false;
    last_user_ = rec_dat->user;
    last_time_ = rec_dat->time;
    last_id_ = rec_dat->id;
    return false;
  }
}

double SvdppModel::compute_norm(int user_activity_size){
  //copied from AsymmetricFactorModel::compute_norm
  double norm = 1.0;
  if(norm_type_=="constant" or norm_type_=="recency") norm = pow(user_activity_size,-0.5);//1.0/sqrt(user_activity_size);
  if(norm_type_=="exponential") norm = pow((pow(gamma_,user_activity_size)-1)/(gamma_-1),-0.5);
  if(norm_type_=="disabled" or norm_type_=="youngest") norm = 1.0;
  cached_norm_ = norm;
  return norm;
}

vector<double> SvdppModel::compute_histvector_sum(RecDat* rec_dat, const vector<const RecDat*>* user_history){
   vector<double> sum_vector(dimension_,0);
   double weight = 1.0; //"constant" or "disabled"
   cached_weights_.clear();
   for(auto it = user_history->rbegin(); it!=user_history->rend(); it++){ 
     if(norm_type_=="recency"){
       double time_diff = rec_dat->time - (*it)->time;
       weight = 604800.0 / (604800.0 + time_diff);
       if(weight > 1) throw runtime_error("timediff should be nonnegative");
       if(weight < 0.01) break; //do not add small components
     }
     cached_weights_.push_back(weight); //for the gradient updater
     Util::sum_update_with(&sum_vector,history_item_factors_.get((*it)->item),weight);
     if(norm_type_=="exponential"){
       weight*=gamma_;
     }
     if(norm_type_=="youngest"){
       weight=0;
     }
   }
   return sum_vector; //ha lassu, akkor ellenorizni kell, hogy itt masol, vagy kioptimalizalja
}


void SvdppModel::write(ostream& file){
  user_factors_.write(file);
  item_factors_.write(file);
  history_item_factors_.write(file);
}

void SvdppModel::read(istream& file){
  user_factors_.read(file);
  item_factors_.read(file);  
  history_item_factors_.read(file);
}


//double SvdppModel::user_factor_mean() {
//  double avg=0;
//  vector<int> user_indices = user_factors_.get_nonnull_indices();
//  for(std::vector<int>::iterator it = user_indices.begin() ; it != user_indices.end(); ++it)
//    avg+=Util::scalar_product(user_factors_.get(*it),user_factors_.get(*it));
//  if(user_indices.size()>0) avg=avg/double(user_indices.size());
//  return avg;
//}
//
//
//double SvdppModel::item_factor_mean() {
//  double avg=0;
//  vector<int> item_indices = item_factors_.get_nonnull_indices();
//  for(std::vector<int>::iterator it = item_indices.begin() ; it != item_indices.end(); ++it)
//    avg+=Util::scalar_product(item_factors_.get(*it),item_factors_.get(*it));
//  if(item_indices.size()>0) avg=avg/double(item_indices.size());
//  return avg;
//}
