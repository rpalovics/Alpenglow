#ifndef DOUBLE_LAYER_MODELUPDATER 
#define DOUBLE_LAYER_MODELUPDATER

#include <vector>
#include "ModelUpdater.h"
#include "../models/CombinedModel.h"

using namespace std;

struct CombinedDoubleLayerModelGradientUpdaterParameters{
  double learningRate;
  double regularizationRate;
  double globalLearningRate;
  double globalRegularizationRate;
  bool alwaysLearn;
  double startCombinationLearningTime;
};
class CombinedDoubleLayerModelGradientUpdater : public ModelGradientUpdater {
  public:
    CombinedDoubleLayerModelGradientUpdater(CombinedDoubleLayerModelGradientUpdaterParameters* params){
      learningRate = params->learningRate;
      regularizationRate = params->regularizationRate;
      globalRegularizationRate = params->globalRegularizationRate;
      globalLearningRate = params->globalLearningRate;
      alwaysLearn = params->alwaysLearn;
      model = NULL;
      startCombinationLearningTime = params->startCombinationLearningTime;
    }
    ~CombinedDoubleLayerModelGradientUpdater(){};
    void set_model(CombinedModel* model_){
      model = model_;
    }
    void update(RecDat * recDat, double gradient);
  private:
    CombinedModel * model;
    double learningRate, regularizationRate, globalLearningRate, globalRegularizationRate;
    bool alwaysLearn;
    double startCombinationLearningTime;
};

#endif
