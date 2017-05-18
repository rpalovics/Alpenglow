#ifndef SIMPLE_LEARNER
#define SIMPLE_LEARNER

#include "OnlineLearner.h"

class SimpleLearner : public OnlineLearner {
  public:
    virtual void learn(RecDat* rec_dat);
    bool self_test(){
      bool ok = OnlineLearner::self_test();
      if (model_simple_updaters_.size()==0){ cerr << "SimpleLearner::model_simple_updaters_ is empty." << endl; }
      if (model_gradient_updaters_.size()!=0){ cerr << "W ARNING: SimpleLearner::model_gradient_updaters_ should be empty as these updaters are not called by the learner." << endl; }
      if (model_ == NULL){ ok=false; cerr << "SimpleLearner::model is not set." << endl; }
      return ok;
    }
};

#endif
