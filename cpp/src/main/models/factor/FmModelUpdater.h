#ifndef FM_MODEL_UPDATER_H
#define FM_MODEL_UPDATER_H

#include "../ModelUpdater.h"
#include "FmModel.h"

using namespace std;

struct FmModelUpdaterParameters{
  double learning_rate;
};

class FmModelUpdater : public Updater{
  public:
    FmModelUpdater(FmModelUpdaterParameters *parameters) : learning_rate_(parameters->learning_rate){};
    void update(RecDat* rec_dat) override;
    void set_model(FmModel* model){model_ = model;};
    bool self_test(){
      bool OK = true;
      if(model_ == NULL){
        OK = false;
        cerr << "FmModelUpdater model_ not set." << endl;
      }
      return OK;
    }
  protected:
    double learning_rate_;
    FmModel* model_ = NULL;
};

#endif

