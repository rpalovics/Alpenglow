#ifndef PERIODIC_DELAYED_LEARNER_WRAPPER
#define PERIODIC_DELAYED_LEARNER_WRAPPER


#include "OnlineLearner.h"
#include <exception>
#include <queue>
using namespace std;
struct LearnerPeriodicDelayedWrapperParameters{
  double period=-1;
  double delay=-1;
};

class LearnerPeriodicDelayedWrapper : public OnlineLearner{
  public:
    LearnerPeriodicDelayedWrapper(LearnerPeriodicDelayedWrapperParameters* params){
      period_ = params->period;
      delay_ = params->delay;
      start_time_ = -1;
      last_period_num_ = 0;
      wrapped_learner_ = NULL;
    }
    virtual ~LearnerPeriodicDelayedWrapper(){}

    virtual void set_wrapped_learner(Updater* learner);

    virtual void learn(RecDat* rec_dat) override;
    virtual void add_simple_updater(ModelSimpleUpdater* model_updater) override;
    virtual void add_gradient_updater(ModelGradientUpdater* model_updater) override;
    virtual void add_multi_updater(ModelMultiUpdater* model_updater) override;
    virtual void set_model(Model* model) override;

    bool self_test(){
      bool ok = OnlineLearner::self_test();
      if(wrapped_learner_ == NULL){
        cerr << "LearnerPeriodicDelayedWrapper::wrapped_learner is not set." << endl; 
        ok = false;
      }
      return ok;
    }
  protected:
    virtual void delayed_learn(RecDat*);
    virtual void periodic_learn(vector<RecDat*>);
    virtual void simple_learn(RecDat*);
    int last_period_num_;
    double period_;
    double delay_;
    double start_time_;
    vector<RecDat*> period_accumulator_;
    queue<RecDat*> delay_accumulator_;
    Updater* wrapped_learner_;
};

#endif
