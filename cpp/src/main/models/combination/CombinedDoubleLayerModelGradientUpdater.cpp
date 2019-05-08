#include "CombinedDoubleLayerModelGradientUpdater.h"

void CombinedDoubleLayerModelGradientUpdater::update(RecDat* rec_dat, double gradient){ //double layer
  if(rec_dat->time>start_combination_learning_time){ //learn combination when models are stable
    vector<double> model_predictions(model->models.size());
    for(uint ii=0; ii<model_predictions.size(); ii++){
      model_predictions[ii] = model->models[ii]->prediction(rec_dat);
      if (!always_learn and model_predictions[ii] == 0) return;
    }
    vector<double>* user_weights = NULL;
    if(model->use_user_weights){ user_weights=model->user_weights[rec_dat->user]; }
    for(uint ii=0; ii<model_predictions.size(); ii++){
      if(model->use_user_weights){
        (*user_weights)[ii] -= gradient * model_predictions[ii] * learning_rate + learning_rate * regularization_rate * (*user_weights)[ii];
      }
      model->global_weights[ii] -= gradient * model_predictions[ii] * global_learning_rate + global_learning_rate * global_regularization_rate * model->global_weights[ii];
    }
  }
}
