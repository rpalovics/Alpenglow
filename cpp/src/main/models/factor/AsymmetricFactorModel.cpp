#include "AsymmetricFactorModel.h"

void AsymmetricFactorModel::set_parameters(AsymmetricFactorModelParameters* parameters){
  factors_parameters_.begin_min=parameters->begin_min;
  factors_parameters_.begin_max=parameters->begin_max;
  factors_parameters_.dimension=parameters->dimension;
  factors_parameters_.seed = parameters->seed;
  history_item_factors_parameters_ = factors_parameters_;
  history_item_factors_parameters_.seed +=1;
  history_item_factors_.set_parameters(history_item_factors_parameters_);
  item_factors_.set_parameters(factors_parameters_);
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
  invalidate_user_factor_=true;
}


void AsymmetricFactorModel::add(RecDat* rec_dat){
  history_item_factors_.init(rec_dat->item);
  item_factors_.init(rec_dat->item);
  //invalidate_user_factor_=true; //elvileg itt nem kell
}

double AsymmetricFactorModel::prediction(RecDat* rec_dat){
  compute_user_factor(rec_dat);
  //if(rec_dat->user==18){ //DEBUG
  //  auto item_vector = item_factors_.get(rec_dat->item);
  //  cerr << *rec_dat;
  //  if(item_vector!=NULL){
  //    for(auto x : *item_vector){ //DEBUG
  //      cerr << " " << x; //DEBUG
  //    } //DEBUG
  //    cerr << endl; //DEBUG
  //  } else {
  //    cerr << "Item vector was NULL." << endl;
  //  }
  //} //DEBUG
  double val = Util::scalar_product(&cached_user_factor_,item_factors_.get(rec_dat->item));
  if(use_sigmoid_)
    return Util::sigmoid_function(val);
  else return val;
}

void AsymmetricFactorModel::compute_user_factor(RecDat * rec_dat){
  if(cache_is_valid(rec_dat)) return; //do not recompute user vector during evaluation
  auto user_history = user_history_container_.get_user_history(rec_dat->user);
  if(user_history!=NULL && user_history->size()!=0){ //TODO should not happen that uh size == 0
    double norm = compute_norm(user_history->size());
    cached_user_factor_ = compute_histvector_sum(rec_dat,user_history); 
    Util::multiply_vector(norm,&cached_user_factor_);
  } else {
    Util::zero_out_vector(&cached_user_factor_);
  }
}
bool AsymmetricFactorModel::cache_is_valid(RecDat* rec_dat){
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
double AsymmetricFactorModel::compute_norm(int user_activity_size){
  double norm = 1.0;
  if(norm_type_=="constant" or norm_type_=="recency") norm = pow(user_activity_size,-0.5);//1.0/sqrt(user_activity_size);
  if(norm_type_=="exponential") norm = pow((pow(gamma_,user_activity_size)-1)/(gamma_-1),-0.5);
  if(norm_type_=="disabled" or norm_type_=="youngest") norm = 1.0;
  cached_norm_ = norm;
  return norm;
}
vector<double> AsymmetricFactorModel::compute_histvector_sum(RecDat* rec_dat, const vector<const RecDat*>* user_history){
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
   //if(rec_dat->user==18){ //DEBUG
   //  cerr << *rec_dat << "history size=" << user_history->size() << " w.size()=" << cached_weights_.size();
   //  for(auto x : sum_vector){ //DEBUG
   //    cerr << " " << x; //DEBUG
   //  } //DEBUG
   //  cerr << endl; //DEBUG
   //} //DEBUG
   return sum_vector; //ha lassu, akkor ellenorizni kell, hogy itt masol, vagy kioptimalizalja
}
void AsymmetricFactorModel::write(ostream& file){
  user_history_container_.write(file);
  history_item_factors_.write(file);
  item_factors_.write(file);
}
void AsymmetricFactorModel::read(istream& file){
  user_history_container_.read(file);
  history_item_factors_.read(file);
  item_factors_.read(file);
}
