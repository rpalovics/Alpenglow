#include "ObjectiveBPR.h"

pair <double,double> ObjectiveBPR::getGradient(RecPred * recPred1, RecPred* recPred2){
  double predDiff = recPred1->prediction - recPred2->prediction;
  double gradient = 0;
//  if (predDiff>0)  gradient = -1 *  exp(-predDiff) / (double) (1 + exp(-predDiff));
  gradient = -1 *  1 / (double) (1 + exp(predDiff));
  return make_pair(gradient,-gradient);
}

