#ifndef OFFLINE_LEARNER
#define OFFLINE_LEARNER

//#include "../models/Model.h"
#include "../recommender_data/RecommenderData.h"

class OfflineLearner{
 public:
  virtual ~OfflineLearner(){}; 
  virtual void iterate()=0; //deprecated
  virtual void fit(RecommenderData* recommender_data){ throw 1;} //TODO make it pure virtual
  bool self_test(){
    bool OK = true;
    return OK;
  }
 protected:
};

#endif
