#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include <vector>

using namespace std;

class VectorInt {
public: 
  vector<int> vec;
  VectorInt(vector<int> vec_){vec=vec_;};
};

#endif