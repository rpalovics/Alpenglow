#include "EigenFactorsLempContainer.h"

#include <stdexcept>
#include <tuple>
#include <iostream>
#include <eigen3/Eigen/Dense>

EigenFactorsLempContainer::EigenFactorsLempContainer(EigenFactors* item_factors, int bucket_size)
: LempContainer(bucket_size) {
  reinitialize(item_factors, bucket_size);
}

void EigenFactorsLempContainer::reinitialize(EigenFactors* item_factors, int bucket_size){
  bucket_size_ = bucket_size;
  item_factors_ = item_factors;
  auto items = get_factor_items();
  rebuild_from_items(items);
}

vector<tuple<int,double,vector<double>>> EigenFactorsLempContainer::factor_to_items(EigenFactors *factor){
  vector<tuple<int,double,vector<double>>> items;
  for(uint i=0; i<item_factors_->factors.rows(); i++){
    if(!item_factors_->seen[i]){
      continue;
    }
    double norm = item_factors_->factors.row(i).norm();
    vector<double> unit_vec;
    if(norm == 0){
      unit_vec = vector<double>(item_factors_->factors.row(i).cols(), 0);
    } else {
      MatrixXdRM row;
      row = item_factors_->factors.row(i)/norm;
      unit_vec = vector<double>(row.data(), row.data()+row.cols());
    }
    items.push_back(make_tuple(i, norm, unit_vec));
  }
  return items;
}

vector<double> EigenFactorsLempContainer::get_factor_item(int id){
  if(id >= item_factors_->factors.rows()){
    throw invalid_argument("Trying to insert not initialized item");
  }
  MatrixXdRM row = item_factors_->factors.row(id);
  return vector<double>(row.data(), row.data()+row.cols());
}

vector<tuple<int,double,vector<double>>> EigenFactorsLempContainer::get_factor_items(){
  return factor_to_items(item_factors_);
};