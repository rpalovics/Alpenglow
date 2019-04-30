#ifndef LEARNER_PERIODIC_DELAYED_WRAPPER_H
#define LEARNER_PERIODIC_DELAYED_WRAPPER_H


#include "../general_interfaces/Updater.h"
#include <exception>
#include <queue>
using namespace std;
struct LearnerPeriodicDelayedWrapperParameters {
  double period=-1;
  double delay=-1;
};

class LearnerPeriodicDelayedWrapper : public Updater{
  public:
    LearnerPeriodicDelayedWrapper(LearnerPeriodicDelayedWrapperParameters* params){
      period_ = params->period;
      delay_ = params->delay;
      start_time_ = -1;
      last_period_num_ = 0;
      wrapped_learner_ = NULL;
    }
    virtual ~LearnerPeriodicDelayedWrapper(){}

    void set_wrapped_learner(Updater* learner);
    virtual void update(RecDat* rec_dat) override;

    bool self_test(){
      bool ok = Updater::self_test();
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

#endif /* LEARNER_PERIODIC_DELAYED_WRAPPER_H */
