#ifndef MASS_PREDICTOR_H
#define MASS_PREDICTOR_H

#include "models/Model.h"
#include "recommender_data/RecommenderData.h"

using namespace std;

class MassPredictor{
public:
  void set_model(Model *model){
    model_ = model;
  }
  vector<double> predict(vector<int> users, vector<int> items){
    RecDat rd;
    int len = min(users.size(), items.size());
    vector<double> predictions(len);
    for(int i=0; i<len; i++){
      rd.user = users[i];
      rd.item = items[i];
      predictions[i] = model_->prediction(&rd);
    }
    return predictions;
  }
protected:
  Model *model_;
};
#endif