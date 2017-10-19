#ifndef FACTOR_MODEL_READER_H
#define FACTOR_MODEL_READER_H

#include <fstream>
#include <vector>
#include <string>
#include "models/factor/FactorModel.h"
#include "models/factor/EigenFactorModel.h"

using namespace std;

struct FactorRepr{
  int entity;
  vector<double> factors;
};
struct UserItemFactors{
  vector<FactorRepr> user_factors;
  vector<FactorRepr> item_factors;
};
class FactorModelReader{
public:
  UserItemFactors read(string file_name, int dimension){
    ifstream ifs(file_name, ios::binary);
    FactorsParameters p;
    p.begin_min = -0.01;
    p.begin_max = 0.01;
    p.dimension = dimension;
    Factors userf(p);
    Factors itemf(p);
    userf.read(ifs);
    itemf.read(ifs);
    ifs.close();

    UserItemFactors uifactors;

    for(uint i=0;i<userf.factors.size();i++){
      if(userf.factors[i]!=NULL){
        FactorRepr user_vector;
        user_vector.entity=i;
        for(int j=0;j<userf.dimension;j++){
          user_vector.factors.push_back(userf.factors[i]->at(j));
        }
        uifactors.user_factors.push_back(user_vector);
      }
    }

    for(uint i=0;i<itemf.factors.size();i++){
      if(itemf.factors[i]!=NULL){
        FactorRepr item_vector;
        item_vector.entity=i;
        for(int j=0;j<itemf.dimension;j++){
          item_vector.factors.push_back(itemf.factors[i]->at(j));
        }
        uifactors.item_factors.push_back(item_vector);
      }
    }
    return uifactors;
  }
};

class EigenFactorModelReader{
public:
  UserItemFactors read(string file_name){
    ifstream ifs(file_name, ios::binary);
    EigenFactorModelParameters p;
    EigenFactorModel m(&p);
    m.read(ifs);
    ifs.close();

    UserItemFactors uifactors;

    auto &user_factors = m.get_user_factors();
    auto &item_factors = m.get_item_factors();

    for(uint i=0;i<user_factors.factors.rows();i++){
      FactorRepr user_vector;
      user_vector.entity=i;
      for(int j=0;j<user_factors.factors.cols();j++){
        user_vector.factors.push_back(user_factors.factors(i,j));
      }
      uifactors.user_factors.push_back(user_vector);
    }

    for(uint i=0;i<item_factors.factors.rows();i++){
      FactorRepr item_vector;
      item_vector.entity=i;
      for(int j=0;j<item_factors.factors.cols();j++){
        item_vector.factors.push_back(item_factors.factors(i,j));
      }
      uifactors.item_factors.push_back(item_vector);
    }

    return uifactors;
  }
};

#endif
