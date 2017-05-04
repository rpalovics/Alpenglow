#include "ObjectiveMSE.h"

double ObjectiveMSE::getGradient(RecPred * recPred){
  //cerr << "getGradient " << recPred->prediction << " " << recPred->score << endl;
  return  recPred->prediction - recPred->score;
}
