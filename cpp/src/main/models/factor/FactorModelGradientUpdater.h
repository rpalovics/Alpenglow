#ifndef FACTOR_MODEL_GRADIENT_UPDATER_H
#define FACTOR_MODEL_GRADIENT_UPDATER_H

#include "../ModelUpdater.h"
#include "FactorModel.h"

using namespace std;

struct FactorModelGradientUpdaterParameters {
  bool turn_off_user_factor_updates = false;
  bool turn_off_item_factor_updates = false;
  bool turn_off_user_bias_updates = false;
  bool turn_off_item_bias_updates = false;
  double learning_rate = 0.1, regularization_rate = 0;
  double learning_rate_bias = 0.1, regularization_rate_bias = 0;
};

class FactorModelGradientUpdater : public ModelGradientUpdater{
  public:
    FactorModelGradientUpdater(FactorModelGradientUpdaterParameters* parameters):
      turn_off_user_factor_updates_(parameters->turn_off_user_factor_updates),
      turn_off_item_factor_updates_(parameters->turn_off_item_factor_updates),
      turn_off_user_bias_updates_(parameters->turn_off_user_bias_updates),
      turn_off_item_bias_updates_(parameters->turn_off_item_bias_updates),
      learning_rate_(parameters->learning_rate),
      learning_rate_bias_(parameters->learning_rate_bias),
      regularization_rate_(parameters->regularization_rate),//should not set to -1 even if missing
      regularization_rate_bias_(parameters->regularization_rate_bias)
    {
    }
    virtual void update(RecDat *rec_dat, double gradient) override;
    void set_model(FactorModel* model){ model_ = model; }
    bool self_test(){
      bool ok = ModelGradientUpdater::self_test();
      if(model_==NULL){ ok=false; cerr << "FactorModelGradientUpdater::model is not set." << endl; }
      if(learning_rate_<0){ ok=false; cerr << "FactorModelGradientUpdater::learning_rate_<0." << endl; }
      if(regularization_rate_<0){ ok=false; cerr << "FactorModelGradientUpdater::regularization_rate<0." << endl; }
      if(model_!=NULL and (model_->use_item_bias_ or model_->use_user_bias_) and learning_rate_bias_<0){ ok=false; cerr << "FactorModelGradientUpdater::learning_rate_bias_<0 and model contains bias." << endl; }
      if(model_!=NULL and (model_->use_item_bias_ or model_->use_user_bias_) and regularization_rate_bias_<0){ ok=false; cerr << "FactorModelGradientUpdater::regularization_rate_bias_<0." << endl; }
      return ok;
    }
  private:
    //parameters
    const double learning_rate_;
    const double learning_rate_bias_;
    const double regularization_rate_;
    const double regularization_rate_bias_;
    const bool turn_off_user_factor_updates_;
    const bool turn_off_item_factor_updates_;
    const bool turn_off_user_bias_updates_;
    const bool turn_off_item_bias_updates_;
    //components
    FactorModel* model_ = NULL;
    //other
    void update_factors(RecDat* rec_dat, double gradient);
    void update_biases(RecDat* rec_dat, double gradient);
};

#endif /* FACTOR_MODEL_GRADIENT_UPDATER_H */
