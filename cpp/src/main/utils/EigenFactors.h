#ifndef EIGEN_FACTORS_H
#define EIGEN_FACTORS_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <gtest/gtest_prod.h>
#include "Random.h"
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXdRM;

struct EigenFactorsParameters {
  int seed;
};

class EigenFactors{
  public:
    EigenFactors(){};
    EigenFactors(EigenFactorsParameters *parameters){
      set_parameters(parameters);
    };
    void write(ostream& file);
    void read(istream& file);
    void set_parameters(EigenFactorsParameters *parameters){
      seed_ = parameters->seed;
      random_ = Random(seed_);
    }
    void reinitialize_factors(int dimension, int users, double begin_min_, double begin_max_);
    void resize(int dimension, int users, double begin_min_, double begin_max_);
    MatrixXdRM factors;
    vector<bool> seen;
    bool self_test(){
      return random_.self_test();
    }
  private:
    Random random_;
    int seed_;
};
#endif /* EIGEN_FACTORS_H */
