#ifndef OBJECTIVE_MSE
#define OBJECTIVE_MSE

#include "Objective.h"


class ObjectiveMSE : public ObjectivePointWise{
  public:
    double get_gradient(RecPred* rec_pred);
};

#endif
