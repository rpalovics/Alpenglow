#ifndef OBJECTIVESIGMOIDMSE_H_
#define OBJECTIVESIGMOIDMSE_H_

#include "../utils/Util.h"
#include "Objective.h"

class ObjectiveSigmoidMSE : public ObjectivePointWise {
public:
  double get_gradient(RecPred* rec_pred);
};


#endif /* OBJECTIVESIGMOIDMSE_H_ */
