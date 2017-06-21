#ifndef POPULARITY_TIMEFRAME_MODEL
#define POPULARITY_TIMEFRAME_MODEL

#include "../Model.h"
#include "../../utils/PopContainers.h"
#include <gtest/gtest_prod.h>

using namespace std;
struct PopularityTimeFrameModelParameters{
  double tau;
};
class PopularityTimeFrameModel : public Model {
  public:
    PopularityTimeFrameModel(PopularityTimeFrameModelParameters* params){
      tau_ = params->tau;
      maxitem_ = 0;
    }
    void add(RecDat* rec_dat) override {}
    double prediction(RecDat* rec_dat) override;
    bool self_test(){
      return Model::self_test();
    }
  private:
    PopContainer items_;
    int maxitem_;
    double tau_;
    friend class PopularityTimeFrameModelUpdater;
};

#endif
