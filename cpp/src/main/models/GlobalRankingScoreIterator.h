#ifndef MODEL_FILTER_H
#define MODEL_FILTER_H

#include "../recommender_data/RecommenderData.h"
#include "Model.h"

class GlobalRankingScoreIterator { //SIP_ABSTRACT
public:
  virtual void run(RecDat* rec_dat){}
  virtual vector<pair<int,double>>* get_global_items()=0;
  virtual vector<pair<int,double>>* get_global_users()=0;
  virtual ~GlobalRankingScoreIterator(){}
  bool self_test(){
    bool OK = true;
    return OK;
  }
};

#endif /* MODEL_FILTER_H */
