#ifndef OFFLINE_LEARNER
#define OFFLINE_LEARNER

#include "../models/Model.h"

class OfflineLearner{
 public:
  OfflineLearner(){}; 
  virtual void iterate()=0;
  bool self_test(){
    bool OK = true;
    return OK;
  }
 protected:
};

#endif
