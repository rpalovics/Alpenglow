#include "EigenFactors.h"

void EigenFactors::reinitialize_factors(int dimension, int max_id, double begin_min, double begin_max){
  factors = MatrixXd::Constant(max_id+1,dimension,0);
  for(int j=0; j<dimension; j++){
    for(int i=0; i<=max_id; i++){
      factors(i,j)=rnd.get()*(begin_max-begin_min)+begin_min;
    }
  }
}

void EigenFactors::resize(int dimension, int max_id, double begin_min, double begin_max){
  if(factors.rows() == 0 || factors.cols() == 0){
    this->reinitialize_factors(dimension, max_id, begin_min, begin_max);
    return;
  }
  int old_cols = factors.cols();
  int old_rows = factors.rows();
  factors.conservativeResize(max_id+1,dimension);

  for(int j=0; j<dimension; j++){
    for(int i=(j>old_cols-1)?0:old_cols; i<=max_id; i++){
      factors(i,j)=rnd.get()*(begin_max-begin_min)+begin_min;
    }
  }
}