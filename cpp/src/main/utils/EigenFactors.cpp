#include "EigenFactors.h"

void EigenFactors::reinitialize_factors(int dimension, int users, double begin_min, double begin_max){
  factors = MatrixXdRM::Constant(users,dimension,0);
  for(int j=0; j<dimension; j++){
    for(int i=0; i<users; i++){
      factors(i,j)=random_.get()*(begin_max-begin_min)+begin_min;
    }
  }
}

void EigenFactors::resize(int dimension, int users, double begin_min, double begin_max){
  if(factors.rows() == 0 || factors.cols() == 0){
    this->reinitialize_factors(dimension, users, begin_min, begin_max);
    return;
  }
  int old_cols = factors.cols();
  int old_rows = factors.rows();
  
  factors.conservativeResize(users,dimension);

  for(int j=0; j<dimension; j++){
    for(int i=(j>=old_cols)?0:old_rows; i<users; i++){
      factors(i,j)=random_.get()*(begin_max-begin_min)+begin_min;
    }
  }
}

void EigenFactors::write(ostream& file){
  typename MatrixXdRM::Index rows=factors.rows(), cols=factors.cols();
  file.write((char*) (&rows), sizeof(typename MatrixXdRM::Index));
  file.write((char*) (&cols), sizeof(typename MatrixXdRM::Index));
  file.write((char*) factors.data(), rows*cols*sizeof(typename MatrixXdRM::Scalar) );
};
void EigenFactors::read(istream& file){
  factors = MatrixXdRM();
  typename MatrixXdRM::Index rows=0, cols=0;
  file.read((char*) (&rows),sizeof(typename MatrixXdRM::Index));
  file.read((char*) (&cols),sizeof(typename MatrixXdRM::Index));
  factors.resize(rows, cols);
  file.read( (char *) factors.data() , rows*cols*sizeof(typename MatrixXdRM::Scalar) );
};
