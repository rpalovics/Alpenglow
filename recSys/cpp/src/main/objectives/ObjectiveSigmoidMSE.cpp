/*
 * ObjectiveSigmoidMSE.cpp
 *
 *  Created on: 2014.04.15.
 *      Author: kisstom
 */

#include "ObjectiveSigmoidMSE.h"

double ObjectiveSigmoidMSE::getGradient(RecPred * recPred) {
  return (recPred->prediction - recPred->score) *
      util.logisticFunction(recPred->prediction);
}


