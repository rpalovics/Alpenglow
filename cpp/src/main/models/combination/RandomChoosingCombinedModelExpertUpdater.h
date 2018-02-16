#ifndef RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER
#define RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER

#include "RandomChoosingCombinedModel.h"
#include "../ModelUpdater.h"
#include <gtest/gtest_prod.h>

struct RandomChoosingCombinedModelExpertUpdaterParameters{
  double eta = 0.1;
};

class RandomChoosingCombinedModelExpertUpdater{
  public:
    void set_model(RandomChoosingCombinedModel* model){model_ = model; }
    void update(RecDat* rec_dat);
    bool self_test(){
      bool ok = true;
      if(model_==NULL){
        ok=false;
        cerr<<"RandomChoosingCombinedModelExpertUpdater::model is not set."<<endl;
      }
      return ok;
    }

  private:
    RandomChoosingCombinedModel* model_ = NULL;
};


#endif
