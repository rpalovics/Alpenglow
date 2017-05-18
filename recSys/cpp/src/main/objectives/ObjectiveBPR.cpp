#include "ObjectiveBPR.h"

pair <double,double> ObjectiveBPR::get_gradient(RecPred * rec_pred1, RecPred* rec_pred2){
  double predDiff = rec_pred1->prediction - rec_pred2->prediction;
  double gradient = 0;
//  if (predDiff>0)  gradient = -1 *  exp(-predDiff) / (double) (1 + exp(-predDiff));
  gradient = -1 *  1 / (double) (1 + exp(predDiff));
  return make_pair(gradient,-gradient);
}

