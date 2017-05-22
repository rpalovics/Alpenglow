#ifndef A_FACTOR_MODEL_GRADIENT_UPDATER
#define A_FACTOR_MODEL_GRADIENT_UPDATER

#include <iomanip>
#include "ModelUpdater.h"
#include "../models/AsymmetricFactorModel.h"

using namespace std;

struct AsymmetricFactorModelGradientUpdaterParameters{
  double learning_rate;
  //TODO regularization rate?
  bool cumulative_item_updates;
  AsymmetricFactorModelGradientUpdaterParameters(){
    learning_rate=-1;
    cumulative_item_updates=false;
  }
};

class AsymmetricFactorModelGradientUpdater : public ModelGradientUpdater{
  public:
    AsymmetricFactorModelGradientUpdater(AsymmetricFactorModelGradientUpdaterParameters* parameters)
      :learning_rate_(parameters->learning_rate)
      ,cumulative_item_updates_(parameters->cumulative_item_updates)
      ,model_(NULL)
    { }
    ~AsymmetricFactorModelGradientUpdater(){};
    void update(RecDat* rec_dat, double gradient) override;
    void beginning_of_updating_cycle(RecDat*) override;
    void end_of_updating_cycle(RecDat*) override;
    void set_model(AsymmetricFactorModel* model){
      model_ = model;
      cumulated_histvector_updates_.resize(model_->dimension_,0);
    }
    bool self_test(){
      bool ok=ModelGradientUpdater::self_test();
      if(model_==NULL){ ok=false; cerr << "AsymmetricFactorModelGradientUpdater::model_ is not set." << endl; }
      if(learning_rate_<0){ ok=false; cerr << "AsymmetricFactorModelGradientUpdater::learning_rate_<1." << endl; }
      return ok;
    }
  private:
    void update_history_item_factors(RecDat* rec_dat, double gradient, vector<double>* item_vector);
    void update_item_factors(RecDat* rec_dat, double gradient);

    //parameters
    const double learning_rate_; 
    const bool cumulative_item_updates_;
    AsymmetricFactorModel* model_;

    //state
    vector<double> cumulated_histvector_updates_;
    //double avg_mse_; //DEBUG
    //int n_of_s_; //DEBUG
};

#endif
