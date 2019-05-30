#include "FactorModel.h"

void FactorModel::set_parameters(FactorModelParameters* parameters){
  FactorsParameters factors_parameters;
  factors_parameters.begin_min=begin_min_;
  factors_parameters.begin_max=begin_max_;
  factors_parameters.dimension=dimension_;
  factors_parameters.seed=parameters->seed+1; //subobject seed: own seed+1
  user_factors_.set_parameters(factors_parameters);
  factors_parameters.seed=parameters->seed+2; //second subobject: own seed+2
  item_factors_.set_parameters(factors_parameters);
}

void FactorModel::clear(){
  user_factors_.clear();
  item_factors_.clear();
  if(use_user_bias_){
    user_bias_.clear();
  }
  if(use_user_bias_){
    item_bias_.clear();
  }
  if(initialize_all_){
    for(int user=0;user<=max_user_;user++) user_factors_.init(user);
    for(int item=0;item<=max_item_;item++) item_factors_.init(item);
    if(use_user_bias_){
      for(int user=0;user<=max_user_;user++) user_bias_.init(user);
    }
    if(use_item_bias_){
      for(int item=0;item<=max_item_;item++) item_bias_.init(item);
    }
  }
  //TODO recency.clear()?
}

void FactorModel::add(RecDat *rec_dat){
  user_factors_.init(rec_dat->user);
  item_factors_.init(rec_dat->item);
  if (use_user_bias_) user_bias_.init(rec_dat->user);
  if (use_item_bias_) item_bias_.init(rec_dat->item);
  lemp_container_.schedule_update_item(rec_dat->item);
}

double FactorModel::prediction(RecDat *rec_dat){
  double scalar_product = compute_product(rec_dat);
  double user_bias_val = compute_user_bias(rec_dat);
  double item_bias_val = compute_item_bias(rec_dat);
  if(user_recency_!=NULL){
    double user_recency_val = user_recency_->get(rec_dat->user,rec_dat->time);
    user_bias_val*=user_recency_val;
    scalar_product*=user_recency_val;
  }
  if(item_recency_!=NULL){
    double item_recency_val = item_recency_->get(rec_dat->item,rec_dat->time);
    item_bias_val*=item_recency_val;
    scalar_product+=item_recency_val;
  }
  double prediction = scalar_product+user_bias_val+item_bias_val;
  if(use_sigmoid_){
    prediction=Util::sigmoid_function(prediction);
  }
  return prediction;
}

double FactorModel::similarity(int item1, int item2){
  vector<double>* item1_vector = item_factors_.get(item1);
  vector<double>* item2_vector = item_factors_.get(item2);
  return Util::scalar_product(item1_vector, item2_vector)/(Util::norm(item1_vector)*Util::norm(item2_vector));
}

double FactorModel::compute_user_bias(RecDat *rec_dat){
  double user_bias_val = 0;
  if (use_user_bias_) user_bias_val = user_bias_.get(rec_dat->user);
  return user_bias_val;
}

double FactorModel::compute_item_bias(RecDat *rec_dat){
  double item_bias_val = 0;
  if (use_item_bias_) item_bias_val = item_bias_.get(rec_dat->item);
  return item_bias_val;
}

double FactorModel::compute_product(RecDat *rec_dat){
  return Util::scalar_product(user_factors_.get(rec_dat->user),item_factors_.get(rec_dat->item));
}

void FactorModel::write(ostream& file){
  user_factors_.write(file);
  item_factors_.write(file);
  //TODO bias, recency
}

void FactorModel::read(istream& file){
  user_factors_.read(file);
  item_factors_.read(file);
  lemp_container_.reinitialize(&item_factors_);
  //TODO bias, recency
}

RankingScoreIterator* FactorModel::get_ranking_score_iterator(int u){
  if(use_item_bias_ || use_user_bias_){
    return NULL;
  } else {
    ranking_score_iterator_ = FactorModelRankingScoreIterator(*user_factors_.get(u), &lemp_container_);
    return &ranking_score_iterator_;
  }
}

//double FactorModel::user_factor_mean() {
//  double avg=0;
//  vector<int> user_indices = user_factors_.get_nonnull_indices();
//  for(std::vector<int>::iterator it = user_indices.begin() ; it != user_indices.end(); ++it)
//    avg+=Util::scalar_product(user_factors_.get(*it),user_factors_.get(*it));
//  if(user_indices.size()>0) avg=avg/double(user_indices.size());
//  return avg;
//}
//
//
//double FactorModel::item_factor_mean() {
//  double avg=0;
//  vector<int> item_indices = item_factors_.get_nonnull_indices();
//  for(std::vector<int>::iterator it = item_indices.begin() ; it != item_indices.end(); ++it)
//    avg+=Util::scalar_product(item_factors_.get(*it),item_factors_.get(*it));
//  if(item_indices.size()>0) avg=avg/double(item_indices.size());
//  return avg;
//}
