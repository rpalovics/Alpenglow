#ifndef EIGEN_FACTORS_H
#define EIGEN_FACTORS_H

#include <stdlib.h>
#include <stdio.h>
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

struct EigenFactorsParameters{
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
      seed = parameters->seed;
      rnd = Random(seed);
    }
    void reinitialize_factors(int dimension, int users, double begin_min_, double begin_max_);
    void resize(int dimension, int users, double begin_min_, double begin_max_);
    MatrixXdRM factors;
    vector<bool> seen;
  private:
    Random rnd;
    int seed;
};
#endif
