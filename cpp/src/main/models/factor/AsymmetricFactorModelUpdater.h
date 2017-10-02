#ifndef A_FACTOR_MODEL_UPDATER
#define A_FACTOR_MODEL_UPDATER

#include "../ModelUpdater.h"
#include "AsymmetricFactorModel.h"

class AsymmetricFactorModelUpdater : public ModelSimpleUpdater{
  public:
    AsymmetricFactorModelUpdater(){ model_=NULL;}
    void update(RecDat* rec_dat) override {
      model_->add(rec_dat);
      model_->user_history_container_.update(rec_dat);
      model_->invalidate_user_factor_=true;
    }
    void set_model(AsymmetricFactorModel* model){model_ = model;}
    bool self_test(){
      bool ok=ModelSimpleUpdater::self_test();
      if(model_==NULL){ ok = false; cerr << "ModelSimpleUpdater::model_ is not set."; }
      return ok;
    }
  private:
    AsymmetricFactorModel* model_;
};

#endif

