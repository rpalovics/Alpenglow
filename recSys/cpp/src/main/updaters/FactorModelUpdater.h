#ifndef FACTOR_MODEL_UPDATER
#define FACTOR_MODEL_UPDATER

#include "ModelUpdater.h"
#include "../models/FactorModel.h"

using namespace std;

class FactorModelUpdater : public ModelSimpleUpdater{
  public:
    void update(RecDat* rec_dat) override;
    void set_model(FactorModel* model){model_ = model;};
    //DEPRECATED
    void setModel(FactorModel* model){
      cerr << "FactorModelUpdater::setModel is DEPRECATED, use google code." << endl;
      set_model(model);
    };
  private:
    FactorModel* model_;
};

#endif

