/*
 * util.h
 *
 *  Created on: 2014.03.19.
 *      Author: kisstom
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
using std::vector;
using std::cerr;
using std::endl;

class Util {
public:
  static double scalarProduct(vector <double> * userFact, vector<double> * itemFact) {
    if (userFact == NULL || itemFact == NULL || userFact->size() != itemFact->size()) {
      return 0.0;
    }
    double product=0;
    for(unsigned ii=0; ii<userFact->size(); ii++) {
      product+=(*userFact)[ii]*(*itemFact)[ii];
    }
    return product;
  }

  static void sumVectors(vector<double> * vec1, vector<double> * vec2, vector <double> * vec3) {
    vec3->clear(); 
    if (vec1 == NULL && vec2!=NULL)
      *vec3 = *vec2;
    if (vec1 != NULL && vec2==NULL)
      *vec3 = *vec1;
    if (vec1 !=NULL && vec2 != NULL){
      for(unsigned ii=0; ii<vec1->size(); ii++)
        vec3->push_back((*vec1)[ii]+(*vec2)[ii]);
    }
  }
  static double norm(vector <double> * v) {
    double n = scalarProduct(v, v);
    if (n <= 0.0) return 0.0;
    return sqrt(n);
  }
  static void sum_update_with(vector<double>* updateFactor, vector<double>* otherFactor, double coef1, double coef2);
  static void sum_update_with(vector<double>* updateFactor, vector<double>* otherFactor, double coef);
  static void sum_update_with(vector<double>* updateFactor, vector<double>* otherFactor);

  static void sumUpdateWith(vector<double>* updateFactor, vector<double>* otherFactor, double coef1, double coef2){ sum_update_with(updateFactor,otherFactor,coef1,coef2); }
  static void sumUpdateWith(vector<double>* updateFactor, vector<double>* otherFactor, double coef){ sum_update_with(updateFactor,otherFactor,coef); }
  static void sumUpdateWith(vector<double>* updateFactor, vector<double>* otherFactor){ sum_update_with(updateFactor,otherFactor); }
  static int euclideanAlg(int a, int b);
  static int firstRelPrimeTo(int a);
  static void multiplyVector(double c, vector<double>* v){
    multiply_vector(c,v);
  }
  static void multiply_vector(double c, vector<double>*);
  static void zeroOutVector(vector<double>* v){
    if(v==NULL) return;
    int dimension = v->size();
    v->clear();
    v->resize(dimension,0);
  }
  static double computeGamma(double x, double y, double tau);
  static void printVector(int i, vector<double>* v);
  static void printNorm(int i, vector<double>* v);
  static bool testVector(const vector<double>* v){ //tests if v contains nan/inf values
    bool ok = true;
    if(v!=NULL){
      for(uint i=0;i<v->size();i++){
        if(std::isnan(v->at(i)) or std::isinf(v->at(i))) ok = false;
      }
    }
    return ok;
  }

  static double sigmoidFunction(double x) {
    return 1.0 / (1.0 + exp(-x));
  }
  static double sigmoidDerivativeFunction(double x) {
    return sigmoidFunction(x) * (1.0 - sigmoidFunction(x));
  }

  static double logisticFunction(double x) {
    return x * (1.0 - x);
  }
  
  static void report(int step, int maxIterNum){   
    if(maxIterNum>=10000){
      if(step%(maxIterNum/10000)==0) std::cerr << std::setprecision(4) << ((double)step/(double)maxIterNum*100) <<"\%-";
    }
    if(step==maxIterNum) std::cerr << "OK" << std::endl;
  }
  

};


inline void Util::sum_update_with(vector<double>* updateFactor, vector<double>* otherFactor) {
  if (NULL == updateFactor || NULL == otherFactor) return;

  for (int i = 0; i < (int) updateFactor->size(); ++i) {
    (*updateFactor)[i] += (*otherFactor)[i];
  }
}

inline void Util::sum_update_with(vector<double>* updateFactor, vector<double>* otherFactor, double coef) {
  if (NULL == updateFactor || NULL == otherFactor) return;
  for (int i = 0; i < (int) updateFactor->size(); ++i) {
    (*updateFactor)[i] += (*otherFactor)[i] * coef;
  }
}

inline void Util::multiply_vector(double coef, vector<double>* vect) {
  if (vect == NULL) return;

  for (int i = 0; i < (int) vect->size(); ++i) {
    (*vect)[i] *= coef;
  }
}


#endif /* UTIL_H_ */
