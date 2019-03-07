#ifndef EXPERIMENT_ENVIRONMENT_H
#define EXPERIMENT_ENVIRONMENT_H

//SIP_AUTOCONVERT

#include "../online_experiment/ExperimentEnvironment.h"

class ExperimentEnvironment;

class NeedsExperimentEnvironment{
  public:
    virtual void set_experiment_environment(ExperimentEnvironment*) = 0;
};

#endif /* EXPERIMENT_ENVIRONMENT_H */
