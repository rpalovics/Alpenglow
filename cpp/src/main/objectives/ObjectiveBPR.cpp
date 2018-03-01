#include "ObjectiveBPR.h"

pair <double,double> ObjectiveBPR::get_gradient(RecPred* rec_pred1, RecPred* rec_pred2){
  double pred_diff = rec_pred1->prediction - rec_pred2->prediction;
  double gradient = 0;
//  if (pred_diff>0)  gradient = -1 *  exp(-pred_diff) / (double) (1 + exp(-pred_diff));
  gradient = -1 *  1 / (double) (1 + exp(pred_diff));
  return make_pair(gradient,-gradient);
}

