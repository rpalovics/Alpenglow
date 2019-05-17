#ifndef NEEDS_EXPERIMENT_ENVIRONMENT_H
#define NEEDS_EXPERIMENT_ENVIRONMENT_H

//SIP_AUTOCONVERT

#include "../online_experiment/ExperimentEnvironment.h"

class ExperimentEnvironment;

class NeedsExperimentEnvironment{
  public:
    void set_experiment_environment(const ExperimentEnvironment* experiment_environment){
      experiment_environment_ = experiment_environment;
    }

  protected:
    const ExperimentEnvironment* experiment_environment_ = NULL;
};

#endif /* NEEDS_EXPERIMENT_ENVIRONMENT_H */
