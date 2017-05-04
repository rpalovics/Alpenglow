#ifndef FACTOR_MODEL_GRADIENT_UPDATER
#define FACTOR_MODEL_GRADIENT_UPDATER

#include "ModelUpdater.h"
#include "../models/FactorModel.h"

using namespace std;

struct FactorModelGradientUpdaterParameters{
  bool turn_off_user_factor_updates;
  bool turn_off_item_factor_updates;
  bool turn_off_user_bias_updates;
  bool turn_off_item_bias_updates;
  double learning_rate, regularization_rate;
  double learning_rate_bias, regularization_rate_bias;
  //DEPRECATED
  double learningRate, regularizationRate;
  double learningRateBias, regularizationRateBias;
  FactorModelGradientUpdaterParameters(){ //setting all to jinjactor default value
    turn_off_user_factor_updates=false;turn_off_item_factor_updates=false;
    turn_off_user_bias_updates=false;turn_off_item_bias_updates=false;
    learning_rate=-1;regularization_rate=-1;
    learning_rate_bias=-1;regularization_rate_bias=-1;
  }
};

class FactorModelGradientUpdater : public ModelGradientUpdater{
  public:
    FactorModelGradientUpdater(FactorModelGradientUpdaterParameters* parameters):
      turn_off_user_factor_updates_(parameters->turn_off_user_factor_updates),
      turn_off_item_factor_updates_(parameters->turn_off_item_factor_updates),
      turn_off_user_bias_updates_(parameters->turn_off_user_bias_updates),
      turn_off_item_bias_updates_(parameters->turn_off_item_bias_updates),
      learning_rate_(parameters->learning_rate!=-1?parameters->learning_rate:parameters->learningRate),
      learning_rate_bias_(parameters->learning_rate_bias!=-1?parameters->learning_rate_bias:parameters->learningRateBias),
      regularization_rate_(parameters->regularization_rate!=-1?parameters->regularization_rate:(parameters->regularizationRate!=-1?parameters->regularizationRate:0)),//should not set to -1 even if missing
      regularization_rate_bias_(parameters->regularization_rate_bias!=-1?parameters->regularization_rate_bias:(parameters->regularizationRateBias!=-1?parameters->regularizationRateBias:0))
    {
      model_ = NULL;
      if(parameters->learningRate!=-1 or
          parameters->regularizationRate!=-1 or
          parameters->learningRateBias!=-1 or
          parameters->regularizationRateBias!=-1){
        cerr << "FactorModelGradientUpdater's CamelCase parameters are DEPRECATED, use google code." << endl;
      }
    }
    ~FactorModelGradientUpdater(){};
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
    //DEPRECATED
    void setModel(FactorModel* model){
      cerr << "FactorModelGradientUpdater::setModel is DEPRECATED, use google code." << endl;
      set_model(model);
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
    FactorModel* model_;
    //other
    void update_factors(RecDat* rec_dat, double gradient);
    void update_biases(RecDat* rec_dat, double gradient);
};

#endif
