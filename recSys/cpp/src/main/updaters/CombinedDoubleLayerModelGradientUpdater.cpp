#include "CombinedDoubleLayerModelGradientUpdater.h"

void CombinedDoubleLayerModelGradientUpdater::update(RecDat * rec_dat, double gradient){ //double layer
  if(rec_dat->time>startCombinationLearningTime){ //learn combination when models are stable
    vector<double> modelPredictions(model->models.size());
    for(uint ii=0; ii<modelPredictions.size(); ii++){
      modelPredictions[ii] = model->models[ii]->prediction(rec_dat);
      if (!alwaysLearn and modelPredictions[ii] == 0) return;
    }
    vector<double>* userWeights = NULL;
    if(model->useUserWeights){ userWeights=model->userWeights[rec_dat->user]; }
    for(uint ii=0; ii<modelPredictions.size(); ii++){
      if(model->useUserWeights){
        (*userWeights)[ii] -= gradient * modelPredictions[ii] * learningRate + learningRate * regularizationRate * (*userWeights)[ii];
      }
      model->globalWeights[ii] -= gradient * modelPredictions[ii] * globalLearningRate + globalLearningRate * globalRegularizationRate * model->globalWeights[ii];
    }
  }
}
