#include "FactorsLempContainer.h"

#include <stdexcept>
#include <tuple>
#include <iostream>

FactorsLempContainer::FactorsLempContainer(Factors* item_factors, int bucket_size)
: LempContainer(bucket_size) {
  item_factors_ = item_factors;
  auto items = get_factor_items();
  rebuild_from_items(items);
}

void FactorsLempContainer::reinitialize(Factors *item_factors, int bucket_size){
  item_factors_ = item_factors;
  auto items = get_factor_items();
  rebuild_from_items(items);
}

vector<tuple<int,double,vector<double>>> FactorsLempContainer::factor_to_items(Factors *factor){
  vector<tuple<int,double,vector<double>>> items;
  for(int i=0; i<factor->get_size(); i++){
    vector<double> *f = factor->get(i);
    if(f!=NULL){
      double norm = Util::norm(f);
      if(norm == 0){
        items.push_back(make_tuple(i, 0, vector<double>(f->size(),0)));
      } else {
        vector<double> f_copy(*f);
        Util::multiply_vector(1/norm, &f_copy);
        items.push_back(make_tuple(i, norm, f_copy));
      }
    }
  }
  return items;
}

vector<double> FactorsLempContainer::get_factor_item(int id){
  vector<double> *item_factor_p = item_factors_->get(id);
  if(item_factor_p == NULL){
    throw invalid_argument("Trying to insert not initialized item");
  }
  return vector<double>(*item_factor_p);
}

vector<tuple<int,double,vector<double>>> FactorsLempContainer::get_factor_items(){
  return factor_to_items(item_factors_);
};