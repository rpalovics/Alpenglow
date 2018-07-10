#ifndef POPULARITY_TIMEFRAME_UPDATER
#define POPULARITY_TIMEFRAME_UPDATER

//SIP_AUTOCONVERT

#include <queue>
#include <vector>
#include "../ModelUpdater.h"
#include "PopularityModel.h"
#include <gtest/gtest_prod.h>

using namespace std;
struct PopularityTimeFrameModelUpdaterParameters{
  double tau;
  PopularityTimeFrameModelUpdaterParameters(){
    tau=-1;
  }
};
class PopularityTimeFrameModelUpdater : public Updater {
  public:
    PopularityTimeFrameModelUpdater(PopularityTimeFrameModelUpdaterParameters* params){
      tau_ = params->tau;
      model_ = NULL;
    }
    void set_model(PopularityModel* model){model_ = model;};
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool ok = Updater::self_test();
      if (model_==NULL) ok=false;
      if (tau_<0) ok=false;
      return ok;
    }
  private:
    double tau_;
    queue<RecDat*> time_frame_data_;
    PopularityModel* model_;
};

#endif
