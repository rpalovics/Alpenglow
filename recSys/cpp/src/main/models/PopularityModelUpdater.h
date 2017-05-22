#ifndef POPULARITY_UPDATER
#define POPULARITY_UPDATER

#include "ModelUpdater.h"
#include "../models/PopularityModel.h"
#include <vector>
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityModelUpdater : public ModelSimpleUpdater {
  public:
    PopularityModelUpdater(){
      model_ = NULL;
    }
    void set_model(PopularityModel* model){model_ = model;}
    virtual void update(RecDat* rec_dat);
    bool self_test(){
      bool ok = ModelSimpleUpdater::self_test();
      if(model_==NULL){ ok=false; }
      return ok;
    }
  private:
    PopularityModel* model_;
};



#endif
