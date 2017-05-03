struct PeriodicImplicitGradientLearnerParameters : PeriodicLearnerParameters{
  %TypeHeaderCode
  #include "online_learners/PeriodicImplicitGradientLearner.h"
  %End
  int number_of_iterations;
  bool turn_off_random_shuffling;
  int seed;
  bool learn;
};

class PeriodicImplicitGradientLearner : public PeriodicLearner /Abstract/ {
  %TypeHeaderCode
  #include "online_learners/PeriodicImplicitGradientLearner.h"
  %End
  public:
    PeriodicImplicitGradientLearner(PeriodicImplicitGradientLearnerParameters* params);
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator /Transfer/);
    void set_gradient_computer(GradientComputer* gradient_computer /Transfer/);
    void add_iteration_logger(Logger* logger /Transfer/);
    bool self_test();
};
