#ifndef OFFLINE_LEARNER_H
#define OFFLINE_LEARNER_H

#include "../recommender_data/RecommenderData.h"

class OfflineLearner{
 public:
  virtual ~OfflineLearner(){} 
  virtual void fit(RecommenderData* recommender_data)=0;
  bool self_test(){
    bool OK = true;
    return OK;
  }
 protected:
};

#endif /* OFFLINE_LEARNER_H */
