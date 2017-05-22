#ifndef POPULARITY_TIMEFRAME_UPDATER
#define POPULARITY_TIMEFRAME_UPDATER

#include "ModelUpdater.h"
#include "../models/PopularityTimeFrameModel.h"
#include <list>
#include <vector>
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityTimeFrameModelUpdater : public ModelSimpleUpdater {
  public:
    void set_model(PopularityTimeFrameModel * model_){model = model_;};
    void update(RecDat * rec_dat);
  private:
    PopularityTimeFrameModel * model;
};

#endif
