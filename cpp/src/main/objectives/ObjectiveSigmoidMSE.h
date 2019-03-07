#ifndef OBJECTIVE_SIGMOID_M_S_E_H
#define OBJECTIVE_SIGMOID_M_S_E_H

#include "../utils/Util.h"
#include "Objective.h"

class ObjectiveSigmoidMSE : public ObjectivePointWise {
public:
  double get_gradient(RecPred* rec_pred);
};


#endif /* OBJECTIVE_SIGMOID_M_S_E_H */
