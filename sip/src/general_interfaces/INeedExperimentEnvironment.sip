
class INeedExperimentEnvironment{
  %TypeHeaderCode
  #include "general_interfaces/INeedExperimentEnvironment.h"
  %End
  public:
    virtual void set_experiment_environment(ExperimentEnvironment*)=0;
};
