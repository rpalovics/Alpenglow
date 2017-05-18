/*
 * ObjectiveSigmoidMSE.h
 *
 *  Created on: 2014.04.15.
 *      Author: kisstom
 */

#ifndef OBJECTIVESIGMOIDMSE_H_
#define OBJECTIVESIGMOIDMSE_H_

#include "../utils/Util.h"
#include "Objective.h"

class ObjectiveSigmoidMSE : public ObjectivePointWise {
public:
  double get_gradient(RecPred * rec_pred);
  virtual ~ObjectiveSigmoidMSE() {}
private:
  Util util;
};


#endif /* OBJECTIVESIGMOIDMSE_H_ */
