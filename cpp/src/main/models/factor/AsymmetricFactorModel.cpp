#include "AsymmetricFactorModel.h"

void AsymmetricFactorModel::set_parameters(AsymmetricFactorModelParameters*){
  FactorsParameters factors_parameters;
  factors_parameters.begin_min=begin_min_;
  factors_parameters.begin_max=begin_max_;
  factors_parameters.dimension=dimension_;
  factors_parameters.seed = seed_;
  item_factors_.set_parameters(factors_parameters);
  
  factors_parameters.seed +=1;
  history_item_factors_.set_parameters(factors_parameters);

  cached_user_factor_.resize(dimension_,0);
}

void AsymmetricFactorModel::clear(){
  history_item_factors_.clear();
  item_factors_.clear();
  user_history_container_.clear_all();
  if(initialize_all_){
    for(int item=0;item<=max_item_;item++){
      item_factors_.init(item);
      history_item_factors_.init(item);
    }
  }
  Util::zero_out_vector(&cached_user_factor_);
  cached_weights_.clear();
  cached_norm_=1;
  last_user_=-1;
  last_time_=-1;
  last_id_=-1;
  cache_marked_invalid_=true;
}

void AsymmetricFactorModel::add(RecDat* rec_dat){
  history_item_factors_.init(rec_dat->item);
  item_factors_.init(rec_dat->item);
}

double AsymmetricFactorModel::prediction(RecDat* rec_dat){
  vector<double>* user_factor = compute_user_factor(rec_dat);
  vector<double>* item_factor = item_factors_.get(rec_dat->item);
  double val = Util::scalar_product(user_factor,item_factor);
  if(use_sigmoid_) return Util::sigmoid_function(val);
  else return val;
}

vector<double>* AsymmetricFactorModel::compute_user_factor(RecDat* rec_dat){
  if( cache_is_valid(rec_dat) ) return &cached_user_factor_; //caching to improve efficiency
  auto user_history = user_history_container_.get_user_history(rec_dat->user);
  bool user_has_history = user_history!=NULL && user_history->size()!=0; //TODO should not happen that uh size == 0
  if(user_has_history){
    double norm = compute_norm(user_history->size());
    cached_user_factor_ = sum_user_history(rec_dat,user_history); 
    Util::multiply_vector(norm,&cached_user_factor_);
  } else {
    Util::zero_out_vector(&cached_user_factor_);
  }
  set_cache_id(rec_dat);
  return &cached_user_factor_;
}

bool AsymmetricFactorModel::cache_is_valid(RecDat* rec_dat) const {
  bool sample_differs = last_user_!=rec_dat->user
      || last_time_!=rec_dat->time
      || last_id_!=rec_dat->id;
  return (!cache_marked_invalid_ && !sample_differs);
}

void AsymmetricFactorModel::set_cache_id(RecDat* rec_dat){
  cache_marked_invalid_=false;
  last_user_ = rec_dat->user;
  last_time_ = rec_dat->time;
  last_id_ = rec_dat->id;
}

double AsymmetricFactorModel::compute_norm(int hist_length){
  double norm = 1.0;
  if(norm_type_=="constant" or norm_type_=="recency") norm = pow(hist_length,-0.5);//1.0/sqrt(hist_length);
  if(norm_type_=="exponential") norm = pow((pow(gamma_,hist_length)-1)/(gamma_-1),-0.5);
  if(norm_type_=="disabled" or norm_type_=="youngest") norm = 1.0;
  cached_norm_ = norm;
  return norm;
}

vector<double> AsymmetricFactorModel::sum_user_history(RecDat* rec_dat,
    const vector<const RecDat*>* user_history){
  vector<double> sum_vector(dimension_,0);
  double weight = 1.0; //"constant" or "disabled"
  cached_weights_.clear();
  for(auto it = user_history->rbegin(); it!=user_history->rend(); it++){ 
    if(norm_type_=="recency"){
      double time_diff = rec_dat->time - (*it)->time;
      weight = 604800.0 / (604800.0 + time_diff); //TODO remove magic constant
      if(weight > 1) throw runtime_error("timediff should be nonnegative");
      if(weight < 0.01) break; //do not add small components
      //TODO remove magic constant
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
  return sum_vector;
}

void AsymmetricFactorModel::write(ostream& file){
  user_history_container_.write(file); //not implemented, throws exception
  history_item_factors_.write(file);
  item_factors_.write(file);
}

void AsymmetricFactorModel::read(istream& file){
  //TODO invalidate model here
  user_history_container_.read(file); //not implemented, throws exception
  history_item_factors_.read(file);
  item_factors_.read(file);
}
