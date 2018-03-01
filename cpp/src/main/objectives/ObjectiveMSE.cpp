#include "ObjectiveMSE.h"

double ObjectiveMSE::get_gradient(RecPred* rec_pred){
  return  rec_pred->prediction - rec_pred->score;
}
