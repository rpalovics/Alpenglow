#ifndef DOUBLE_LAYER_MODELUPDATER 
#define DOUBLE_LAYER_MODELUPDATER

#include <vector>
#include "ModelUpdater.h"
#include "../models/CombinedModel.h"

using namespace std;

struct CombinedDoubleLayerModelGradientUpdaterParameters{
  double learning_rate;
  double regularization_rate;
  double global_learning_rate;
  double global_regularization_rate;
  bool always_learn;
  double start_combination_learning_time;
};
class CombinedDoubleLayerModelGradientUpdater : public ModelGradientUpdater {
  public:
    CombinedDoubleLayerModelGradientUpdater(CombinedDoubleLayerModelGradientUpdaterParameters* params){
      learning_rate = params->learning_rate;
      regularization_rate = params->regularization_rate;
      global_regularization_rate = params->global_regularization_rate;
      global_learning_rate = params->global_learning_rate;
      always_learn = params->always_learn;
      model = NULL;
      start_combination_learning_time = params->start_combination_learning_time;
    }
    ~CombinedDoubleLayerModelGradientUpdater(){};
    void set_model(CombinedModel* model_){
      model = model_;
    }
    void update(RecDat * rec_dat, double gradient);
  private:
    CombinedModel * model;
    double learning_rate, regularization_rate, global_learning_rate, global_regularization_rate;
    bool always_learn;
    double start_combination_learning_time;
};

#endif
