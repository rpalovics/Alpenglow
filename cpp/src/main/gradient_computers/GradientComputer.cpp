#include "GradientComputer.h"

double GradientComputerPointWise::get_gradient(RecDat* rec_dat){
  //compute prediction
  RecPred rec_pred;
  rec_pred.prediction = model_->prediction(rec_dat);
  rec_pred.score = rec_dat->score;
  //compute gradient
  double gradient = objective_->get_gradient(&rec_pred);
  return gradient;
}

// vector<pair<RecDat*,double> >* GradientComputerImplicitPairWise::get_next_gradient(){
//   //get next negative sample
//   RecDat* negative_data = &(*train_data_it);
//   train_data_it++;
//   //compute predictions
//   RecPred positive_pred;
//   positive_pred.prediction = model_->prediction(positive_data);
//   positive_pred.score = positive_data->score;
//   RecPred negative_pred;
//   negative_pred.prediction = model_->prediction(negative_data);
//   negative_pred.score = negative_data->score;
//   //compute gradient
//   pair<double,double> gradient_pair = objective_->get_gradient(&positive_pred, &negative_pred);
//   //gradient_vector
//   gradient_vector.clear();
//   gradient_vector.push_back(make_pair(positive_data,gradient_pair.first));
//   gradient_vector.push_back(make_pair(negative_data,gradient_pair.second));
//   return &gradient_vector;
// }

// vector<pair<RecDat*,double> >* GradientComputerListWise::get_next_gradient(){
//   has_next_=false;
//   //compute predictions
//   vector<RecPred> predictions;
//   predictions.resize(train_data->size());
//   for(uint i=0; i<train_data->size(); i++){
//     predictions[i].prediction = model_->prediction(&(train_data->at(i)));
//     predictions[i].score = train_data->at(i).score;
//   }
//   //compute gradients
//   vector<double> gradients;
//   gradients = objective_->get_gradient(&predictions);
//   //gradient_vector
//   gradient_vector.clear();
//   gradient_vector.resize(train_data->size());
//   for(uint i=0; i<train_data->size(); i++){
//     gradient_vector[i].first = &(train_data->at(i));
//     gradient_vector[i].second = gradients[i];
//   }
//   return &gradient_vector;
// }

