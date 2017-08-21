#include <gtest/gtest.h>
#include "../../main/models/baseline/PersonalPopularityModel.h"
#include "../../main/models/baseline/PersonalPopularityModelUpdater.h"

namespace {
class TestPersonalPopularityModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;
    PersonalPopularityModel model;
    PersonalPopularityModelUpdater modelUpdater;

    TestPersonalPopularityModel(){}
    virtual ~TestPersonalPopularityModel(){}
    virtual void SetUp(){
      modelUpdater.set_model(&model);
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
        delete *it;
      }
    }
    RecDat* createRecDat(int user, int item, double time, double score){
      RecDat* recDat = new RecDat();
      recDat -> user = user;
      recDat -> item = item;
      recDat -> time = time;
      recDat -> score = score;
      recDats.push_back(recDat);
      return recDat;
    }
};
}

TEST_F(TestPersonalPopularityModel, update){
  EXPECT_EQ(0, model.prediction(createRecDat(1,100,0,0)));
  RecDat* recDat = createRecDat(1,100,0.0,1.0);
  model.add(recDat);
  modelUpdater.update(recDat);
  EXPECT_EQ(1, model.prediction(createRecDat(1,100,0,0)));

  recDat = createRecDat(1,100,0.0,1.0);
  model.add(recDat);
  modelUpdater.update(recDat);
  EXPECT_EQ(1, model.prediction(createRecDat(1,100,0,0)));

  recDat = createRecDat(1,101,0.0,1.0);
  model.add(recDat);
  modelUpdater.update(recDat);
  model.add(recDat);
  modelUpdater.update(recDat);
  model.add(recDat);
  modelUpdater.update(recDat);
  EXPECT_DOUBLE_EQ(log(3+log(3)/log(4))/log(4+1), model.prediction(createRecDat(1,100,0,0)));
  EXPECT_EQ(1, model.prediction(createRecDat(1,101,0,0)));

  recDat = createRecDat(2,100,0.0,1);
  for(int i=0; i<10; i++){
    model.add(recDat);
    modelUpdater.update(recDat);
  }
  EXPECT_DOUBLE_EQ(log(3+1)/log(4+1), model.prediction(createRecDat(1,100,0,0)));
  EXPECT_EQ(log(4+log(4)/log(13))/log(4+1), model.prediction(createRecDat(1,101,0,0)));
  EXPECT_EQ(1, model.prediction(createRecDat(2,100,0,0)));

  EXPECT_DOUBLE_EQ(1, model.prediction(createRecDat(3,100,0,0)));
  EXPECT_DOUBLE_EQ(log(1+log(4)/log(13))/log(2), model.prediction(createRecDat(3,101,0,0)));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
