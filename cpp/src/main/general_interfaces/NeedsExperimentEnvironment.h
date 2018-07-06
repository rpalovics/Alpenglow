#ifndef I_NEEDEXPERIMENTENVIRONMENT
#define I_NEEDEXPERIMENTENVIRONMENT

//SIP_AUTOCONVERT

#include "../online_experiment/ExperimentEnvironment.h"

class ExperimentEnvironment;

class NeedsExperimentEnvironment{
  public:
    virtual void set_experiment_environment(ExperimentEnvironment*) = 0;
};

#endif
