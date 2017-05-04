#include "GradientComputer.h"

vector<pair<RecDat*,double> >* GradientComputerPointWise::get_next_gradient(){
  //get next sample
  RecDat* recDat = &(*trainDataIt);
  trainDataIt++;
  double gradient = get_gradient(recDat);
  //gradientVector
  gradientVector.clear();
  gradientVector.push_back(make_pair(recDat,gradient));
  return &gradientVector;
}

double GradientComputerPointWise::get_gradient(RecDat* recDat){
  //compute prediction
  RecPred recPred;
  recPred.prediction = model->prediction(recDat);
  recPred.score = recDat->score;
  //compute gradient
  double gradient = objective->getGradient(&recPred);
  return gradient;
}

vector<pair<RecDat*,double> >* GradientComputerImplicitPairWise::get_next_gradient(){
  //get next negative sample
  RecDat* negativeData = &(*trainDataIt);
  trainDataIt++;
  //compute predictions
  RecPred positivePred;
  positivePred.prediction = model->prediction(positiveData);
  positivePred.score = positiveData->score;
  RecPred negativePred;
  negativePred.prediction = model->prediction(negativeData);
  negativePred.score = negativeData->score;
  //compute gradient
  pair<double,double> gradientPair = objective->getGradient(&positivePred, &negativePred);
  //gradientVector
  gradientVector.clear();
  gradientVector.push_back(make_pair(positiveData,gradientPair.first));
  gradientVector.push_back(make_pair(negativeData,gradientPair.second));
  return &gradientVector;
}

vector<pair<RecDat*,double> >* GradientComputerListWise::get_next_gradient(){
  hasNext_=false;
  //compute predictions
  vector<RecPred> predictions;
  predictions.resize(trainData->size());
  for(uint i=0; i<trainData->size(); i++){
    predictions[i].prediction = model->prediction(&(trainData->at(i)));
    predictions[i].score = trainData->at(i).score;
  }
  //compute gradients
  vector<double> gradients;
  gradients = objective->getGradient(&predictions);
  //gradientVector
  gradientVector.clear();
  gradientVector.resize(trainData->size());
  for(uint i=0; i<trainData->size(); i++){
    gradientVector[i].first = &(trainData->at(i));
    gradientVector[i].second = gradients[i];
  }
  return &gradientVector;
}

