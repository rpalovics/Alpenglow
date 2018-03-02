#include <gtest/gtest.h>
#include "../../main/models/combination/AbsoluteErrorEvaluator.h"
#include "../../main/models/combination/DCGEvaluator.h"

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
    vector<RecDat*> rec_dats;
    TestAbsoluteErrorEvaluator(){}
    virtual ~TestAbsoluteErrorEvaluator(){}
    virtual void SetUp(){
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = rec_dats.begin();it!=rec_dats.end();it++){
        delete *it;
      }
    }
    RecDat* create_rec_dat(int user, int item, double time, double score){
      RecDat* rec_dat = new RecDat;
      rec_dat -> user = user;
      rec_dat -> item = item;
      rec_dat -> time = time;
      rec_dat -> score = score;
      rec_dats.push_back(rec_dat);
      return rec_dat;
    }
};
class DummyModel2 : public Model {
  public:
    double prediction(RecDat* rec_dat){
      int item=rec_dat->item;
      if(item>=(int)my_predictions_.size()) return 0;
      return my_predictions_[item];
    }
    vector<double> my_predictions_;
};
class TestDCGEvaluator : public ::testing::Test { 
  public:
    DummyModel2 model;
    DCGEvaluator evaluator;
    vector<RecDat*> rec_dats;
    TestDCGEvaluator(){}
    virtual ~TestDCGEvaluator(){}
    virtual void SetUp(){
      for(int i=1;i<10;i++){
        model.my_predictions_.push_back(1.0/i);
      }
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = rec_dats.begin();it!=rec_dats.end();it++){
        delete *it;
      }
    }
    RecDat* create_rec_dat(int user, int item, double time, double score){
      RecDat* rec_dat = new RecDat;
      rec_dat -> user = user;
      rec_dat -> item = item;
      rec_dat -> time = time;
      rec_dat -> score = score;
      rec_dats.push_back(rec_dat);
      return rec_dat;
    }
};
}

TEST_F(TestAbsoluteErrorEvaluator, test){
  evaluator.set_model(&model);
  ASSERT_TRUE(evaluator.self_test());
  for(int i=3;i<100;i+=7){
    int j=i%9;
    model.my_prediction_=i+j;
    RecDat* rec_dat = create_rec_dat(1,2,3,i);
    double loss = evaluator.get_loss(rec_dat);
    EXPECT_EQ(j,loss);
    model.my_prediction_=i-j;
    loss = evaluator.get_loss(rec_dat);
    EXPECT_EQ(j,loss);
  }
}

TEST_F(TestDCGEvaluator, test){
  evaluator.set_model(&model);
  ASSERT_TRUE(evaluator.self_test());
  //TODO
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
