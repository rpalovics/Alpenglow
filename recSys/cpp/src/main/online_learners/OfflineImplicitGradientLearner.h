#ifndef OFFLINE_IMPLICIT_GRADIENT_LEARNER
#define OFFLINE_IMPLICIT_GRADIENT_LEARNER

#include "PeriodicImplicitGradientLearner.h"
#include "../negative_sample_generators/NegativeSampleGenerator.h"


using namespace std;


struct OfflineImplicitGradientLearnerParameters : public PeriodicImplicitGradientLearnerParameters{};

class OfflineImplicitGradientLearner : public PeriodicImplicitGradientLearner {
  public:
    OfflineImplicitGradientLearner(OfflineImplicitGradientLearnerParameters* params):PeriodicImplicitGradientLearner(params){} 
    bool self_test(){
      bool ok = PeriodicImplicitGradientLearner::self_test();
      return ok;
    }
  private:
    vector<RecDat*> generate_train_data(RecDat* rec_dat) override;
};

#endif
