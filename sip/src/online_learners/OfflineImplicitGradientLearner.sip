struct OfflineImplicitGradientLearnerParameters : public PeriodicImplicitGradientLearnerParameters{
  %TypeHeaderCode
  #include "online_learners/OfflineImplicitGradientLearner.h"
  %End
};

class OfflineImplicitGradientLearner : public PeriodicImplicitGradientLearner {
  %TypeHeaderCode
  #include "online_learners/OfflineImplicitGradientLearner.h"
  %End
  public:
    OfflineImplicitGradientLearner(OfflineImplicitGradientLearnerParameters* params); 
    bool self_test();
};
