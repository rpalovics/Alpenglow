#ifndef FACTOR_MODEL_UPDATER_H
#define FACTOR_MODEL_UPDATER_H

#include "../ModelUpdater.h"
#include "FactorModel.h"

//SIP_AUTOCONVERT

using namespace std;

class FactorModelUpdater : public Updater{
  public:
    void update(RecDat* rec_dat) override;
    void set_model(FactorModel* model){model_ = model;}
    bool self_test(){
      bool ok = Updater::self_test();
      if (model_==NULL) ok=false;
      return ok;
    }
  private:
    FactorModel* model_ = NULL;
};

#endif /* FACTOR_MODEL_UPDATER_H */
