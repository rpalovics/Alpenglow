#include <gtest/gtest.h>
#include "../../main/models/combination/AbsoluteErrorEvaluator.h"

namespace {

class DummyModel : public Model {
  public:
    double prediction(RecDat*){ return my_prediction_; }
    double my_prediction_ = 0.5;
};
class TestAbsoluteErrorEvaluator : public ::testing::Test { 
  public:
    DummyModel model;
    AbsoluteErrorEvaluator evaluator;
    vector<RecDat*> recDats;
    TestAbsoluteErrorEvaluator(){}
    virtual ~TestAbsoluteErrorEvaluator(){}
    virtual void SetUp(){
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
        delete *it;
      }
    }
    RecDat* createRecDat(int user, int item, double time, double score){
      RecDat* recDat = new RecDat;
      recDat -> user = user;
      recDat -> item = item;
      recDat -> time = time;
      recDat -> score = score;
      recDats.push_back(recDat);
      return recDat;
    }
};
}

TEST_F(TestAbsoluteErrorEvaluator, update){
  evaluator.set_model(&model);
  ASSERT_TRUE(evaluator.self_test());
  for(int i=3;i<100;i+=7){
    int j=i%9;
    model.my_prediction_=i+j;
    RecDat* rec_dat = createRecDat(1,2,3,i);
    double loss = evaluator.get_loss(rec_dat);
    EXPECT_EQ(j,loss);
    model.my_prediction_=i-j;
    loss = evaluator.get_loss(rec_dat);
    EXPECT_EQ(j,loss);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
