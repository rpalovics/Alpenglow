#include "EigenFactorModel.h"


double EigenFactorModel::prediction(RecDat* rec_dat){
  return user_factors_.factors.row(rec_dat->user)*item_factors_.factors.row(rec_dat->item).transpose();
}

void EigenFactorModel::resize(int users, int items){
  if(user_factors_.factors.rows() < users){
    user_factors_.resize(dimension_, users, begin_min_, begin_max_);
  }
  if(item_factors_.factors.rows() < items){
    item_factors_.resize(dimension_, items, begin_min_, begin_max_);
  }
}

void EigenFactorModel::clear(){
  user_factors_.factors = MatrixXdRM();
  item_factors_.factors = MatrixXdRM();
}


void EigenFactorModel::write(ofstream& file){
  file.write((char*) (&dimension_), sizeof(dimension_));
  file.write((char*) (&seed_), sizeof(seed_));
  file.write((char*) (&begin_min_), sizeof(begin_min_));
  file.write((char*) (&begin_max_), sizeof(begin_max_));
  user_factors_.write(file);
  item_factors_.write(file);
};

void EigenFactorModel::read(ifstream& file){
  file.read((char*) (&dimension_), sizeof(dimension_));
  file.read((char*) (&seed_), sizeof(seed_));
  file.read((char*) (&begin_min_), sizeof(begin_min_));
  file.read((char*) (&begin_max_), sizeof(begin_max_));
  user_factors_.read(file);
  item_factors_.read(file);
};