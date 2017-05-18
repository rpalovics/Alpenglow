#include "ObjectiveMSE.h"

double ObjectiveMSE::get_gradient(RecPred * rec_pred){
  //cerr << "get_gradient " << rec_pred->prediction << " " << rec_pred->score << endl;
  return  rec_pred->prediction - rec_pred->score;
}
