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
    ExperimentEnvironment experiment_environment;
    DummyModel2 model;
    vector<RecDat*> rec_dats;
    vector<int> items;
    TestDCGEvaluator(){}
    virtual ~TestDCGEvaluator(){}
    virtual void SetUp(){
      model.my_predictions_.push_back(0);
      for(int i=1;i<10;i++){
        model.my_predictions_.push_back(1.0/i);
        items.push_back(i);
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
  DCGEvaluatorParameters params;
  params.top_k=10;
  DCGEvaluator evaluator(&params);
  evaluator.set_model(&model);
  evaluator.set_experiment_environment(&experiment_environment);
  ASSERT_TRUE(evaluator.initialize());
  ASSERT_TRUE(evaluator.self_test());
  for(int item : items){
    RecDat* rec_dat = create_rec_dat(9,item,9,9);
    experiment_environment.update(rec_dat);
  }
  for(int item : items){
    RecDat* rec_dat = create_rec_dat(10,item,10,10);
    double x = evaluator.get_score(rec_dat);
    int rank = round(1.0/model.prediction(rec_dat));
    double dcg = log(2.0)/log(rank+1);
    EXPECT_DOUBLE_EQ(dcg,x);
  }
  std::random_shuffle(model.my_predictions_.begin()+1,model.my_predictions_.end());
  for(int item : items){
    RecDat* rec_dat = create_rec_dat(10,item,10,10);
    double x = evaluator.get_score(rec_dat);
    int rank = round(1.0/model.prediction(rec_dat));
    double dcg = log(2.0)/log(rank+1);
    EXPECT_DOUBLE_EQ(dcg,x);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
