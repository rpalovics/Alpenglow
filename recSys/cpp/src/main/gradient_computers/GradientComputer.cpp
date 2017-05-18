#include "GradientComputer.h"

vector<pair<RecDat*,double> >* GradientComputerPointWise::get_next_gradient(){
  //get next sample
  RecDat* rec_dat = &(*train_dataIt);
  train_dataIt++;
  double gradient = get_gradient(rec_dat);
  //gradientVector
  gradientVector.clear();
  gradientVector.push_back(make_pair(rec_dat,gradient));
  return &gradientVector;
}

double GradientComputerPointWise::get_gradient(RecDat* rec_dat){
  //compute prediction
  RecPred rec_pred;
  rec_pred.prediction = model->prediction(rec_dat);
  rec_pred.score = rec_dat->score;
  //compute gradient
  double gradient = objective->get_gradient(&rec_pred);
  return gradient;
}

vector<pair<RecDat*,double> >* GradientComputerImplicitPairWise::get_next_gradient(){
  //get next negative sample
  RecDat* negativeData = &(*train_dataIt);
  train_dataIt++;
  //compute predictions
  RecPred positivePred;
  positivePred.prediction = model->prediction(positiveData);
  positivePred.score = positiveData->score;
  RecPred negativePred;
  negativePred.prediction = model->prediction(negativeData);
  negativePred.score = negativeData->score;
  //compute gradient
  pair<double,double> gradientPair = objective->get_gradient(&positivePred, &negativePred);
  //gradientVector
  gradientVector.clear();
  gradientVector.push_back(make_pair(positiveData,gradientPair.first));
  gradientVector.push_back(make_pair(negativeData,gradientPair.second));
  return &gradientVector;
}

vector<pair<RecDat*,double> >* GradientComputerListWise::get_next_gradient(){
  has_next_=false;
  //compute predictions
  vector<RecPred> predictions;
  predictions.resize(train_data->size());
  for(uint i=0; i<train_data->size(); i++){
    predictions[i].prediction = model->prediction(&(train_data->at(i)));
    predictions[i].score = train_data->at(i).score;
  }
  //compute gradients
  vector<double> gradients;
  gradients = objective->get_gradient(&predictions);
  //gradientVector
  gradientVector.clear();
  gradientVector.resize(train_data->size());
  for(uint i=0; i<train_data->size(); i++){
    gradientVector[i].first = &(train_data->at(i));
    gradientVector[i].second = gradients[i];
  }
  return &gradientVector;
}

