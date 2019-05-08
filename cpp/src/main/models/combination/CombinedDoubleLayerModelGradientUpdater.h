#ifndef COMBINED_DOUBLE_LAYER_MODEL_GRADIENT_UPDATER_H
#define COMBINED_DOUBLE_LAYER_MODEL_GRADIENT_UPDATER_H

#include <vector>
#include "../ModelUpdater.h"
#include "CombinedModel.h"

//SIP_AUTOCONVERT

using namespace std;

struct CombinedDoubleLayerModelGradientUpdaterParameters {
  double learning_rate = 0.05;
  double regularization_rate = 0.0;
  double global_learning_rate = 0.05;
  double global_regularization_rate = 0.0;
  bool always_learn = true;
  double start_combination_learning_time = 0;
};
class CombinedDoubleLayerModelGradientUpdater : public ModelGradientUpdater {
  public:
    CombinedDoubleLayerModelGradientUpdater(CombinedDoubleLayerModelGradientUpdaterParameters* params){
      learning_rate = params->learning_rate;
      regularization_rate = params->regularization_rate;
      global_regularization_rate = params->global_regularization_rate;
      global_learning_rate = params->global_learning_rate;
      always_learn = params->always_learn;
      start_combination_learning_time = params->start_combination_learning_time;
    }
    void set_model(CombinedModel* model_){ model = model_; }
    void update(RecDat* rec_dat, double gradient);
    bool self_test(){
      bool ok = ModelGradientUpdater::self_test();
      if (model == NULL) ok=false;
      return ok;
    }
  private:
    CombinedModel* model = NULL;
    double learning_rate, regularization_rate, global_learning_rate, global_regularization_rate;
    bool always_learn;
    double start_combination_learning_time;
};

#endif /* COMBINED_DOUBLE_LAYER_MODEL_GRADIENT_UPDATER_H */
