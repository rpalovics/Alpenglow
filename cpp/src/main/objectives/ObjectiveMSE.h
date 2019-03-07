#ifndef OBJECTIVE_M_S_E_H
#define OBJECTIVE_M_S_E_H

//SIP_AUTOCONVERT

#include "Objective.h"


class ObjectiveMSE : public ObjectivePointWise{
  public:
    double get_gradient(RecPred* rec_pred);
};

#endif /* OBJECTIVE_M_S_E_H */
