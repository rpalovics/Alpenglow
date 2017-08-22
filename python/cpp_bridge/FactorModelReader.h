#ifndef FACTOR_MODEL_READER_H
#define FACTOR_MODEL_READER_H

#include <fstream>
#include <vector>
#include <string>
#include "models/factor/FactorModel.h"

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

#endif
