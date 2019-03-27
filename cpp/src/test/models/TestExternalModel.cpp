#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include "../../main/models/ExternalModel.h"
#include "../../main/offline_learners/OfflineExternalModelLearner.h"

namespace {

class TestExternalModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;

    TestExternalModel(){}
    virtual ~TestExternalModel(){}
    virtual void SetUp(){
    }
    RecDat* createRecDat(int user, int item, double time, double score){
      RecDat* recDat = new RecDat;
      recDat -> user = user;
      recDat -> item = item;
      recDat -> time = time;
      recDat -> category = 0;
      recDat -> score = score;
      recDats.push_back(recDat);
      return recDat;
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
        delete *it;
      }
    }
};

}

TEST_F(TestExternalModel, test){

  ExternalModelParameters params;
  params.mode="write";
  ExternalModel model(&params);
  OfflineExternalModelLearnerParameters params2;
  params2.out_name_base = "test_batch";
  params2.mode="write";
  OfflineExternalModelLearner offline_learner(&params2);
  offline_learner.set_model(&model);

  EXPECT_DOUBLE_EQ(1.0,1.0);
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
