#ifndef NEEDS_EXPERIMENT_ENVIRONMENT_H
#define NEEDS_EXPERIMENT_ENVIRONMENT_H

//SIP_AUTOCONVERT

#include "../online_experiment/ExperimentEnvironment.h"

class ExperimentEnvironment;

class NeedsExperimentEnvironment{
  public:
    virtual void set_experiment_environment(ExperimentEnvironment*) = 0;
};

#endif /* NEEDS_EXPERIMENT_ENVIRONMENT_H */
