#ifndef POSNEG_FACTOR_MODEL_GRADIENT_UPDATER
#define POSNEG_FACTOR_MODEL_GRADIENT_UPDATER

#include "../ModelUpdater.h"
#include "FactorModel.h"

using namespace std;

struct PosNegFactorModelGradientUpdaterParameters{
  bool turn_off_user_factor_updates; //turns off user vector updates at all
  bool turn_off_item_factor_updates;
  bool turn_off_user_bias_updates; //turns off user bias updates at all
  bool turn_off_item_bias_updates;
  double learning_rate, regularization_rate;
  double learning_rate_bias, regularization_rate_bias;
  //posneg
  double learning_rate_decrease; //multiplies lr when updating extra samples
  bool turn_on_user_posneg_factor_updates;
  bool turn_on_item_posneg_factor_updates;
  bool turn_on_user_posneg_bias_updates;
  bool turn_on_item_posneg_bias_updates;
  PosNegFactorModelGradientUpdaterParameters(){ //setting all to jinjactor default value
    turn_off_user_factor_updates=false;turn_off_item_factor_updates=false;
    turn_off_user_bias_updates=false;turn_off_item_bias_updates=false;
    learning_rate-=1;regularization_rate=-1;
    learning_rate_bias=-1;regularization_rate_bias=-1;
    learning_rate_decrease=-1;
    turn_on_user_posneg_factor_updates=false;turn_on_item_posneg_factor_updates=false;
    turn_on_user_posneg_bias_updates=false;turn_on_item_posneg_bias_updates=false;
  }
};

class PosNegFactorModelGradientUpdater : public ModelGradientUpdater{
  public:
    PosNegFactorModelGradientUpdater(PosNegFactorModelGradientUpdaterParameters* parameters):
      turn_off_user_factor_updates_(parameters->turn_off_user_factor_updates),
      turn_off_item_factor_updates_(parameters->turn_off_item_factor_updates),
      turn_off_user_bias_updates_(parameters->turn_off_user_bias_updates),
      turn_off_item_bias_updates_(parameters->turn_off_item_bias_updates),
      learning_rate_(parameters->learning_rate),
      learning_rate_bias_(parameters->learning_rate_bias),
      regularization_rate_(parameters->regularization_rate!=-1?parameters->regularization_rate:0),//should not set to -1 even if missing
      regularization_rate_bias_(parameters->regularization_rate_bias!=-1?parameters->regularization_rate_bias:0),
      learning_rate_decrease_(parameters->learning_rate_decrease!=-1?parameters->learning_rate_decrease:1),
      turn_on_user_posneg_factor_updates_(parameters->turn_on_user_posneg_factor_updates),
      turn_on_item_posneg_factor_updates_(parameters->turn_on_item_posneg_factor_updates),
      turn_on_user_posneg_bias_updates_(parameters->turn_on_user_posneg_bias_updates),
      turn_on_item_posneg_bias_updates_(parameters->turn_on_item_posneg_bias_updates)
    {
      model_ = NULL;
    };
    virtual void update(RecDat *rec_dat, double gradient) override;
    void set_model(FactorModel* model){ model_ = model; }
    bool self_test(){
      bool ok = ModelGradientUpdater::self_test();
      if(model_==NULL){ ok=false; cerr << "PosNegFactorModelGradientUpdater::model is not set." << endl; }
      if(learning_rate_<0){ ok=false; cerr << "PosNegFactorModelGradientUpdater::learning_rate_<0." << endl; }
      if(regularization_rate_<0){ ok=false; cerr << "PosNegFactorModelGradientUpdater::regularization_rate<0." << endl; }
      if(model_!=NULL and (model_->use_item_bias_ or model_->use_user_bias_) and learning_rate_bias_<0){ ok=false; cerr << "PosNegFactorModelGradientUpdater::learning_rate_bias_<0 and model contains bias." << endl; }
      if(model_!=NULL and (model_->use_item_bias_ or model_->use_user_bias_) and regularization_rate_bias_<0){ ok=false; cerr << "PosNegFactorModelGradientUpdater::regularization_rate_bias_<0." << endl; }
      if(learning_rate_decrease_<0){ ok=false; cerr << "PosNegFactorModelGradientUpdater::learning_rate_decrease_<0." << endl; }
      return ok;
    }
  private:
    //parameters
    bool turn_off_user_factor_updates_;
    bool turn_off_item_factor_updates_;
    bool turn_off_user_bias_updates_;
    bool turn_off_item_bias_updates_;
    double learning_rate_;
    double learning_rate_bias_;
    double regularization_rate_;
    double regularization_rate_bias_;
    double learning_rate_decrease_;
    bool turn_on_user_posneg_factor_updates_;
    bool turn_on_item_posneg_factor_updates_;
    bool turn_on_user_posneg_bias_updates_;
    bool turn_on_item_posneg_bias_updates_;
    //components
    FactorModel* model_;
    //other
    void update_factors(RecDat* rec_dat, double gradient);
    void update_biases(RecDat* rec_dat, double gradient);
};

#endif
