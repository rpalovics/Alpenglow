#include <gtest/gtest.h>
#include "../../main/models/baseline/PopularityModel.h"
#include "../../main/models/baseline/PopularityTimeFrameModelUpdater.h"

namespace {
class TestPopularityTimeFrameModel : public ::testing::Test { 
  public:
    vector<RecDat*> rec_dats;
    PopularityModel model;

    TestPopularityTimeFrameModel(){}
    virtual ~TestPopularityTimeFrameModel(){}
    virtual void SetUp(){}
    virtual void TearDown(){
      for (RecDat* rec_dat : rec_dats){
        delete rec_dat;
      }
    }
   RecDat* create_rec_dat(int user, int item, double time){
     RecDat* rec_dat = new RecDat;
     rec_dat -> user = user;
     rec_dat -> item = item;
     rec_dat -> time = time;
     rec_dats.push_back(rec_dat);
     return rec_dat;
   }
};
}
TEST_F(TestPopularityTimeFrameModel, test){
  PopularityTimeFrameModelUpdaterParameters params;
  params.tau=10;
  PopularityTimeFrameModelUpdater updater(&params);
  updater.set_model(&model);

}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
