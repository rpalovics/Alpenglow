
class NeedsExperimentEnvironment{
  %TypeHeaderCode
  #include "general_interfaces/NeedsExperimentEnvironment.h"
  %End
  public:
    virtual void set_experiment_environment(ExperimentEnvironment*) = 0;
};

