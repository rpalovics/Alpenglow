#ifndef POPULARITY_TIMEFRAME_MODEL
#define POPULARITY_TIMEFRAME_MODEL

#include "Model.h"
#include <list>
#include <vector>
#include <gtest/gtest_prod.h>

using namespace std;
struct PopularityTimeFrameModelParameters{
  double tau;
};
class PopularityTimeFrameModel : public Model {
  public:
    PopularityTimeFrameModel(PopularityTimeFrameModelParameters* params){
      tau = params->tau;
      maxitem = 0;
    }
    ~PopularityTimeFrameModel(){}
    void add(RecDat * rec_dat);
    double prediction(RecDat * rec_dat);
  private:
    vector<int> items;
    int maxitem;
    list<RecDat*> time_frame_data;
    double tau;
    friend class PopularityTimeFrameModelUpdater;
    FRIEND_TEST(TestPopularityTimeFrameModel, modelsize);
    FRIEND_TEST(TestPopularityTimeFrameModel, update);
    FRIEND_TEST(TestPopularityTimeFrameModel, learn);
};

#endif
