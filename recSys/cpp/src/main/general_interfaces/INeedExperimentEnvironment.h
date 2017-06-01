#ifndef I_NEEDEXPERIMENTENVIRONMENT
#define I_NEEDEXPERIMENTENVIRONMENT

#include "../online_experiment/ExperimentEnvironment.h"

class INeedExperimentEnvironment{
  public:
    virtual void set_experiment_environment(ExperimentEnvironment*) = 0;
};

#endif
