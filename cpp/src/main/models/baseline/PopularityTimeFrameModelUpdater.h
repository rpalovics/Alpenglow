#ifndef POPULARITY_TIME_FRAME_MODEL_UPDATER_H
#define POPULARITY_TIME_FRAME_MODEL_UPDATER_H

//SIP_AUTOCONVERT

#include <queue>
#include <vector>
#include "../ModelUpdater.h"
#include "PopularityModel.h"
#include <gtest/gtest_prod.h>

using namespace std;
struct PopularityTimeFrameModelUpdaterParameters {
  double tau = -1;
};
class PopularityTimeFrameModelUpdater : public Updater {
/**
  Time-aware updater for PopularityModel, which only considers the last tau time interval when calculating popularities. Note that the time window ends at the timestamp of the last updating sample. the timestamp of the sample in the prediction call is not considered.
*/
  public:
    PopularityTimeFrameModelUpdater(PopularityTimeFrameModelUpdaterParameters* params){
      tau_ = params->tau;
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
    PopularityModel* model_ = NULL;
};

#endif /* POPULARITY_TIME_FRAME_MODEL_UPDATER_H */
