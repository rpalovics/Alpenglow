#ifndef SVDPP_MODEL_UPDATER_H
#define SVDPP_MODEL_UPDATER_H

#include "../ModelUpdater.h"
#include "SvdppModel.h"

class SvdppModelUpdater : public Updater{
  public:
    void update(RecDat* rec_dat) override {
      model_->user_history_container_.update(rec_dat);
      model_->invalidate_user_factor_=true;
      model_->add(rec_dat);
    }
    void set_model(SvdppModel* model){model_=model;}
    bool self_test(){
      bool ok = Updater::self_test();
      if (model_==NULL) { ok=false; cerr << "SvdppModelUpdater::model_ is not set." << endl; }
      return ok;
    }
  private:
    SvdppModel* model_ = NULL;
};

#endif /* SVDPP_MODEL_UPDATER_H */
