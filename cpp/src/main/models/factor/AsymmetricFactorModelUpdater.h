#ifndef ASYMMETRIC_FACTOR_MODEL_UPDATER_H
#define ASYMMETRIC_FACTOR_MODEL_UPDATER_H

#include "../ModelUpdater.h"
#include "AsymmetricFactorModel.h"

class AsymmetricFactorModelUpdater : public Updater{
  public:
    void update(RecDat* rec_dat) override {
      model_->add(rec_dat);
      model_->user_history_container_.update(rec_dat);
      model_->invalidate_user_factor_=true;
    }
    void set_model(AsymmetricFactorModel* model){model_ = model;}
    bool self_test(){
      bool ok=Updater::self_test();
      if(model_==NULL){ ok = false; cerr << "Updater::model_ is not set."; }
      return ok;
    }
  private:
    AsymmetricFactorModel* model_ = NULL;
};

#endif /* ASYMMETRIC_FACTOR_MODEL_UPDATER_H */
