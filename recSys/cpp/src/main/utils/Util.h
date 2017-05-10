#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
using std::vector;

class Util {
public:
  static double scalar_product(vector<double>* user_factor, vector<double>* item_factor){
    if (user_factor == NULL || item_factor == NULL || user_factor->size() != item_factor->size()){
      return 0.0;
    }
    double product=0;
    for(unsigned ii=0; ii<user_factor->size(); ii++) {
      product+=(*user_factor)[ii]*(*item_factor)[ii];
    }
    return product;
  }
  static double norm(vector<double>* v){
    double n = scalar_product(v, v);
    if (n <= 0.0) return 0.0;
    return sqrt(n);
  }

  static void sum_update_with(vector<double>* update_factor, vector<double>* other_factor, double coef1, double coef2){
    if (NULL == update_factor || NULL == other_factor) return;
  
    for (int i = 0; i < (int) update_factor->size(); ++i) {
      (*update_factor)[i] = (*update_factor)[i] * coef1 + (*other_factor)[i] * coef2;
    }
  }
  static void sum_update_with(vector<double>* update_factor, vector<double>* other_factor, double coef){
    if (NULL == update_factor || NULL == other_factor) return;
    for (int i = 0; i < (int) update_factor->size(); ++i) {
      (*update_factor)[i] += (*other_factor)[i] * coef;
    }
  }

  static void sum_update_with(vector<double>* update_factor, vector<double>* other_factor){
    if (NULL == update_factor || NULL == other_factor) return;
  
    for (int i = 0; i < (int) update_factor->size(); ++i) {
      (*update_factor)[i] += (*other_factor)[i];
    }
  }

  static void multiply_vector(double c, vector<double>* vect){
    if (vect == NULL) return;
  
    for (int i = 0; i < (int) vect->size(); ++i) {
      (*vect)[i] *= c;
    }
  }

  static void zero_out_vector(vector<double>* v){
    if(v==NULL) return;
    std::fill(v->begin(), v->end(), 0);
  }

  static double compute_gamma(double x, double y, double tau){
    double delta_t = x - y;
    double weight = 1;
    if (delta_t > 0) weight = 1.0 - (log(delta_t)/log(tau));
    return weight;
  }
  static double sigmoid_function(double x) {
    return 1.0 / (1.0 + exp(-x));
  }
  static double sigmoid_derivative_function(double x) {
    return sigmoid_function(x) * (1.0 - sigmoid_function(x));
  }
  static double logistic_function(double x) {
    return x * (1.0 - x);
  }

  //DEPRECATED functions
  static double scalarProduct(vector<double>* user_factor, vector<double>* item_factor){ return scalar_product(user_factor, item_factor); } //DEPRECATED
  static void sumUpdateWith(vector<double>* update_factor, vector<double>* other_factor, double coef1, double coef2){ sum_update_with(update_factor,other_factor,coef1,coef2); } //DEPRECATED
  static void sumUpdateWith(vector<double>* update_factor, vector<double>* other_factor, double coef){ sum_update_with(update_factor,other_factor,coef); } //DEPRECATED
  static void sumUpdateWith(vector<double>* update_factor, vector<double>* other_factor){ sum_update_with(update_factor,other_factor); } //DEPRECATED
  static void multiplyVector(double c, vector<double>* v){ multiply_vector(c,v); } //DEPRECATED
  static void zeroOutVector(vector<double>* v){ zero_out_vector(v); } //DEPRECATED
  static double computeGamma(double x, double y, double tau){ return compute_gamma(x,y,tau); } //DEPRECATED
  static double sigmoidFunction(double x) { return sigmoid_function(x); } //DEPRECATED
  static double sigmoidDerivativeFunction(double x) { return sigmoid_derivative_function(x); } //DEPRECATED
  static double logisticFunction(double x) { return logistic_function(x); } //DEPRECATED
};

#endif /* UTIL_H_ */
