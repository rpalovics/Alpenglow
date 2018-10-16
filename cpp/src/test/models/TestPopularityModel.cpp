#include <gtest/gtest.h>
#include "../../main/models/baseline/PopularityModel.h"
#include "../../main/models/baseline/PopularityModelUpdater.h"

namespace {
class TestPopularityModel : public ::testing::Test { 
  public:
    vector<RecDat*> rec_dats;
    PopularityModel model;
    PopularityModelUpdater updater;

    TestPopularityModel(){}
    virtual ~TestPopularityModel(){}
    virtual void SetUp(){
      updater.set_model(&model);
    }
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
} //namespace

TEST_F(TestPopularityModel, test){
  EXPECT_TRUE(updater.self_test());
  for(int i=3;i<8;i++){
    for(int j=0;j<i;j++){
      RecDat* rec_dat = create_rec_dat(j,i,10*i);
      updater.update(rec_dat);
    }
  }
  double previous_prediction = 1.1;
  for(int item=7;item>2;item--){
    RecDat* rec_dat = create_rec_dat(15,item,100);
    double prediction = model.prediction(rec_dat);
    EXPECT_EQ(log(item+1)/log(8),prediction);
    EXPECT_GT(previous_prediction,prediction);
    previous_prediction = prediction;
  }
  for(int item=8;item<15;item++){
    RecDat* rec_dat = create_rec_dat(15,item,100);
    double prediction = model.prediction(rec_dat);
    EXPECT_EQ(0,prediction);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
