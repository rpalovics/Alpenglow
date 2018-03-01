#include "ObjectiveListRank.h"

vector<double> ObjectiveListRank::get_gradient(vector<RecPred>* _predictions){
  predictions = _predictions;
  clear();
  compute_gradients();
  return gradients;
}

void ObjectiveListRank::compute_gradients(){
  int idx = 0;
  for (vector<RecPred>::iterator it = predictions->begin(); it != predictions->end(); it++) {
    double prediction_exp = exp(sigmoid(it->prediction));
    double score_exp =  exp(it->score);
    predictions_exp[idx] = prediction_exp;
    scores_exp[idx] = score_exp;
    prediction_norm += prediction_exp;
    score_norm += score_exp;
    idx++;
  }
  for(uint ii=0; ii<gradients.size(); ii++){
    gradients[ii] = ( predictions_exp[ii]/(double)prediction_norm - scores_exp[ii]/(double)score_norm )* sigmoid_der((*predictions)[ii].prediction);
  }
}

void ObjectiveListRank::clear(){
  predictions_exp.clear(); scores_exp.clear(); gradients.clear();
  predictions_exp.resize(predictions->size());
  scores_exp.resize(predictions->size());
  gradients.resize(predictions->size());
  prediction_norm = 0;
  score_norm = 0;
}
