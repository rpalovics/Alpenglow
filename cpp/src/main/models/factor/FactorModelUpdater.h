#ifndef FACTOR_MODEL_UPDATER
#define FACTOR_MODEL_UPDATER

#include "../ModelUpdater.h"
#include "FactorModel.h"

using namespace std;

class FactorModelUpdater : public Updater{
  public:
    void update(RecDat* rec_dat) override;
    void set_model(FactorModel* model){model_ = model;};
  private:
    FactorModel* model_;
};

#endif

