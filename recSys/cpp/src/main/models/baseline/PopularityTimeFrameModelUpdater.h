#ifndef POPULARITY_TIMEFRAME_UPDATER
#define POPULARITY_TIMEFRAME_UPDATER

#include <queue>
#include <vector>
#include "../ModelUpdater.h"
#include "PopularityTimeFrameModel.h"
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityTimeFrameModelUpdater : public ModelSimpleUpdater {
  public:
    PopularityTimeFrameModelUpdater(){
      model_ = NULL;
    }
    void set_model(PopularityTimeFrameModel* model){model_ = model;};
    void update(RecDat* rec_dat);
    bool self_test(){
      bool ok = ModelSimpleUpdater::self_test();
      if (model_==NULL) ok=false;
      return ok;
    }
  private:
    queue<RecDat*> time_frame_data_;
    PopularityTimeFrameModel* model_;
};

#endif
