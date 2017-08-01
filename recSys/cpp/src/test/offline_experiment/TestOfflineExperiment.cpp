#include <gtest/gtest.h>
#include "../../main/offline_experiment/OfflineExperiment.h"

namespace {
class DummyOfflineEvaluator : public OfflineEvaluator {
 public:
  DummyOfflineEvaluator(){ counter = 0; }
  void evaluate(){
    counter++;
  }
  int counter;
};
//class DummyModel : public Model{
// public:
//   DummyModel(){ add_counter_=0; }
//  void add(RecDat* rec_dat){add_counter_++;}
//  double prediction(RecDat* rec_dat){
//    rec_data_.push_back(rec_dat);
//    return 0;
//  }
//  int add_counter_;
//  vector<RecDat*> rec_data_;
//};
class DummyOfflineLearner : public OfflineLearner{
 public:
  DummyOfflineLearner(){ counter = 0; }
  void iterate(){
    counter++;
  }
  int counter;
};

class TestOfflineExperiment : public ::testing::Test { 
  public:
    vector<RecDat> recDats;

    TestOfflineExperiment(){}
    virtual ~TestOfflineExperiment(){}
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }
    RecDat* createRecDat(int user, int item, double time){
      RecDat recDat;
      recDat.user = user;
      recDat.item = item;
      recDat.time = time;
      recDats.push_back(recDat);
      return &recDats[recDats.size()-1];
    }
};
}

TEST_F(TestOfflineExperiment, test){
  OfflineExperiment offline_experiment;
  DummyOfflineEvaluator offline_evaluator1;
  DummyOfflineEvaluator offline_evaluator2;
  DummyOfflineEvaluator offline_evaluator3;
  offline_experiment.add_offline_evaluator(&offline_evaluator1);
  offline_experiment.add_offline_evaluator(&offline_evaluator2);
  offline_experiment.add_offline_evaluator(&offline_evaluator3);
  EXPECT_TRUE(offline_experiment.self_test());
  DummyOfflineLearner offline_learner1;
  DummyOfflineLearner offline_learner2;
  DummyOfflineLearner offline_learner3;
  offline_experiment.add_offline_learner(&offline_learner1);
  offline_experiment.add_offline_learner(&offline_learner2);
  offline_experiment.add_offline_learner(&offline_learner3);
  EXPECT_TRUE(offline_experiment.self_test());
  offline_experiment.run();
  EXPECT_EQ(1,offline_learner1.counter);
  EXPECT_EQ(1,offline_learner2.counter);
  EXPECT_EQ(1,offline_learner3.counter);
  EXPECT_EQ(1,offline_evaluator1.counter);
  EXPECT_EQ(1,offline_evaluator2.counter);
  EXPECT_EQ(1,offline_evaluator3.counter);

}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
