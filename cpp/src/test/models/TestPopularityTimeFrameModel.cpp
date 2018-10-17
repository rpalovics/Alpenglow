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

  for(int i=0;i<5;i++){
    RecDat* rec_dat = create_rec_dat(i,i%2,2);
    updater.update(rec_dat); //00011
  }
  double pred0 = model.prediction(create_rec_dat(10,0,5));
  double pred1 = model.prediction(create_rec_dat(10,1,5));
  EXPECT_GT(pred0,pred1);

  for(int i=0;i<5;i++){
    RecDat* rec_dat = create_rec_dat(i,i%2+1,9);
    updater.update(rec_dat); //11122
  }
  pred0 = model.prediction(create_rec_dat(10,0,10));
  pred1 = model.prediction(create_rec_dat(10,1,10));
  double pred2 = model.prediction(create_rec_dat(10,2,10));
  EXPECT_GT(pred1,pred0);
  EXPECT_GT(pred0,pred2);

  RecDat* rec_dat = create_rec_dat(5,3,15);
  updater.update(rec_dat); //3
  pred0 = model.prediction(create_rec_dat(10,0,16));
  pred1 = model.prediction(create_rec_dat(10,1,16));
  pred2 = model.prediction(create_rec_dat(10,2,16));
  double pred3 = model.prediction(create_rec_dat(10,3,16));
  EXPECT_GT(pred1,pred2);
  EXPECT_GT(pred2,pred3);
  EXPECT_GT(pred3,pred0);
  EXPECT_EQ(0,pred0);

  for(int i = 4; i<15; i++){
    double pred = model.prediction(create_rec_dat(10,i,16));
    EXPECT_EQ(0,pred);
  }

}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
