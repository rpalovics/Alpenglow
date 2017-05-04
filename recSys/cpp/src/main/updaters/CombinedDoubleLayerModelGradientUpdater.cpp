#include "CombinedDoubleLayerModelGradientUpdater.h"

void CombinedDoubleLayerModelGradientUpdater::update(RecDat * recDat, double gradient){ //double layer
  if(recDat->time>startCombinationLearningTime){ //learn combination when models are stable
    vector<double> modelPredictions(model->models.size());
    for(uint ii=0; ii<modelPredictions.size(); ii++){
      modelPredictions[ii] = model->models[ii]->prediction(recDat);
      if (!alwaysLearn and modelPredictions[ii] == 0) return;
    }
    vector<double>* userWeights = NULL;
    if(model->useUserWeights){ userWeights=model->userWeights[recDat->user]; }
    for(uint ii=0; ii<modelPredictions.size(); ii++){
      if(model->useUserWeights){
        (*userWeights)[ii] -= gradient * modelPredictions[ii] * learningRate + learningRate * regularizationRate * (*userWeights)[ii];
      }
      model->globalWeights[ii] -= gradient * modelPredictions[ii] * globalLearningRate + globalLearningRate * globalRegularizationRate * model->globalWeights[ii];
    }
  }
}
