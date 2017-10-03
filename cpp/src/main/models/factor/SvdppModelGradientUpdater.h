#ifndef SVD_MODEL_GRADIENT_UPDATER
#define SVD_MODEL_GRADIENT_UPDATER

#include "../ModelUpdater.h"
#include "SvdppModel.h"

using namespace std;

struct SvdppModelGradientUpdaterParameters{
  double learning_rate;
  bool cumulative_item_updates;
  //double regularization_rate; //not implemented
};

class SvdppModelGradientUpdater : public ModelGradientUpdater {
  public:
    SvdppModelGradientUpdater(SvdppModelGradientUpdaterParameters *parameters){
      learning_rate_ = parameters->learning_rate;
      cumulative_item_updates_ = parameters->cumulative_item_updates;
      model_ = NULL;
    };
    void set_model(SvdppModel* model){
      model_ = model;
      cumulated_histvector_updates_.resize(model_->dimension_,0);
    }
    void message(UpdaterMessage message) override;
    void update(RecDat* rec_dat, double gradient) override;
    void beginning_of_updating_cycle(RecDat*) override;
    void end_of_updating_cycle(RecDat*) override;
    bool self_test(){
      bool ok = ModelGradientUpdater::self_test();
      if(model_==NULL){ ok=false; cerr << "SvdppModelGradientUpdater::model is not set." << endl; }
      if(learning_rate_<0){ ok=false; cerr << "SvdppModelGradientUpdater::learning_rate is not set." << endl; }
      return ok;
    }
  private:
    void update_history_item_factors(RecDat* rec_dat, double gradient, vector<double>* item_vector);
    void update_item_factors(RecDat* rec_dat, double gradient);
    void update_user_factors(RecDat* rec_dat, double gradient);

    //parameters
    double learning_rate_;
    bool cumulative_item_updates_;

    //state
    SvdppModel *model_;
    vector<double> cumulated_histvector_updates_;
};

#endif
