struct PeriodicSamplingLearnerParameters : public PeriodicImplicitGradientLearnerParameters{
  %TypeHeaderCode
  #include "online_learners/PeriodicSamplingLearner.h"
  %End
  std::string distribution;
  int number_of_samples;
  double y;
  double geometric_param;
};

class PeriodicSamplingLearner : public PeriodicImplicitGradientLearner{
  %TypeHeaderCode
  #include "online_learners/PeriodicSamplingLearner.h"
  %End
  public:
    PeriodicSamplingLearner(PeriodicSamplingLearnerParameters* params);
    bool self_test();
};