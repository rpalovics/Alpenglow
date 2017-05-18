#include "ObjectiveListRank.h"

vector <double> ObjectiveListRank::get_gradient(vector <RecPred> * _predictions){
  predictions = _predictions;
  clear();
  computeGradients();
  return gradients;
}

void ObjectiveListRank::computeGradients(){
  int idx = 0;
  for (vector<RecPred>::iterator it = predictions->begin(); it != predictions->end(); it++) {
    double predictionExp = exp(sigmoid(it->prediction));
    double scoreExp =  exp(it->score);
    predictionsExp[idx] = predictionExp;
    scoresExp[idx] = scoreExp;
    predictionNorm += predictionExp;
    scoreNorm += scoreExp;
    idx++;
  }
  for(uint ii=0; ii<gradients.size(); ii++){
    gradients[ii] = ( predictionsExp[ii]/(double)predictionNorm - scoresExp[ii]/(double)scoreNorm )* sigmoidDer((*predictions)[ii].prediction);
  }
}

void ObjectiveListRank::clear(){
  predictionsExp.clear(); scoresExp.clear(); gradients.clear();
  predictionsExp.resize(predictions->size());
  scoresExp.resize(predictions->size());
  gradients.resize(predictions->size());
  predictionNorm = 0;
  scoreNorm = 0;
}
