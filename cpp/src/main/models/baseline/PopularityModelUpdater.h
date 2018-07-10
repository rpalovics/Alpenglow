#ifndef POPULARITY_UPDATER
#define POPULARITY_UPDATER

//SIP_AUTOCONVERT

#include <vector>
#include "../ModelUpdater.h"
#include "PopularityModel.h"
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityModelUpdater : public Updater {
  public:
    PopularityModelUpdater(){
      model_ = NULL;
    }
    void set_model(PopularityModel* model){model_ = model;}
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool ok = Updater::self_test();
      if(model_==NULL){ ok=false; }
      return ok;
    }
  private:
    PopularityModel* model_;
};



#endif
