#ifndef PERIODIC_SAMPLING_LEARNER
#define PERIODIC_SAMPLING_LEARNER

#include "PeriodicImplicitGradientLearner.h"
#include <gtest/gtest_prod.h>

struct PeriodicSamplingLearnerParameters : public PeriodicImplicitGradientLearnerParameters{
  string distribution;
  int number_of_samples;
  double y; //parameter of arctg random
  double geometric_param; //parameter of geometric random
};

class PeriodicSamplingLearner : public PeriodicImplicitGradientLearner{
  public:
    PeriodicSamplingLearner(PeriodicSamplingLearnerParameters* params):PeriodicImplicitGradientLearner(params),distribution_(params->distribution),number_of_samples_(params->number_of_samples),y_(params->y),geometric_param_(params->geometric_param){}
    bool self_test(){
      bool ok = PeriodicImplicitGradientLearner::self_test();
      if(distribution_!="uniform" and distribution_!="linear" and distribution_!="arctg" and distribution_!="geometric"){
        cerr << "PeriodicImplicitGradientLearner::distribution should be set to uniform, linear, geometric or arctg." << endl;
        ok = false;
      }
      if(distribution_=="arctg" and y_==-1){
        cerr << "PeriodicImplicitGradientLearner::y_ is not set, y_ is a parameter of arctg random generation." << endl;
        ok = false;
      }
      if(distribution_=="geometric" and geometric_param_==-1){
        cerr << "PeriodicImplicitGradientLearner::y_ is not set, y_ is a parameter of arctg random generation." << endl;
        ok = false;
      }
      return ok;
    }
  private:
    virtual vector<RecDat*> generate_train_data(RecDat* rec_dat) override;
    const string distribution_;
    const int number_of_samples_;
    const double y_; //parameter of arctg random
    const double geometric_param_; //parameter of geometric random
    FRIEND_TEST(TestPeriodicSamplingLearner, uniform);
    FRIEND_TEST(TestPeriodicSamplingLearner, linear);
    FRIEND_TEST(TestPeriodicSamplingLearner, arctg);
};

#endif
