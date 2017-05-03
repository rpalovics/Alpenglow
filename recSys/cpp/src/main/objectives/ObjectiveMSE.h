#ifndef OBJECTIVE_MSE
#define OBJECTIVE_MSE

#include "Objective.h"


class ObjectiveMSE : public ObjectivePointWise{
  public:
    ObjectiveMSE(){};
    ~ObjectiveMSE(){};
    double getGradient (RecPred  * recPred);
  protected:
};

#endif
