#ifndef FACTORS
#define FACTORS

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <gtest/gtest_prod.h>

using namespace std;

struct FactorsParameters{
  double begin_max;
  double begin_min;
  double dimension;
};

class Factors{
  public:
    Factors(){};
    Factors(FactorsParameters parameters){
      set_parameters(parameters);
    };
    ~Factors(){
      for(uint i=0;i<factors.size();i++){
        if(factors[i]!=NULL){
          delete factors[i];
          factors[i]=NULL;
        }
      } 
    }
    void set_parameters(FactorsParameters parameters);
    void init(int idx);
    void initZero(int idx);
    void initRand(int idx);
    vector<double>* get(int idx); 
    void set(int idx, vector<double>* value);
    void setZero(int idx); 
    void setRand(int idx); 
    void set(int idx, int dim, double val); 
    void allToZero();
    void add(int idx, vector<double>* value);
    void add(int idx, vector<double>* value, double coef);
    void add(int idx, int dim, double val);
    //void print(int idx);
    inline void lin_combine(int idx, double weight, vector<double>* other);
    inline void linCombine(int idx, double weight, vector<double>* other){
      lin_combine(idx,weight,other);
    }
    //inline void lin_combine(int idx, double weight, double val);
    //inline void lin_combine(int idx, double weight);
    void resize(int idx);
    int getSize();
    void write(ofstream& file);
    void read(ifstream& file);
    void clear();
    size_t get_dimension(){return dimension;}
    vector <int> getNonnullIndices();
  private:
    vector <vector<double>*> factors;
    double begin_min, begin_max;
    int dimension;
};

void Factors::lin_combine(int idx, double weight, vector<double>* other){
  if(other->size() == (uint) dimension){
    for(int ii=0; ii<dimension; ii++){
      (*factors[idx])[ii] += weight*(*other)[ii];
    }
  }
}

//void Factors::linCombine(int idx, double weight){
//  for(int ii=0; ii<dimension; ii++){
//    //(*factors[idx])[ii]=(*factors[idx])[ii]+weight*val;
//    (*factors[idx])[ii] += weight;
//  }
//}

#endif
