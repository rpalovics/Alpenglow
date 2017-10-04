#include <gtest/gtest.h>
#include "../../main/online_learners/PeriodicOfflineLearnerWrapper.h"
#include "../../main/recommender_data/SimpleIterator.h"
#include <vector>

namespace {
class DummyOfflineLearner : public OfflineLearner {
  void iterate() override {}
  void fit(RecommenderData* recommender_data) override {
    if(no_more_fit) EXPECT_TRUE(false);
  } //TODO
  public:
    bool no_more_fit = false;
};
class DummyPeriodicOfflineLearnerWrapper : public PeriodicOfflineLearnerWrapper {
  public:
    DummyPeriodicOfflineLearnerWrapper(PeriodicOfflineLearnerWrapperParameters* params)
      :PeriodicOfflineLearnerWrapper(params){}
    RecommenderData* generate_recommender_data(RecDat* rec_dat) override { return &recommender_data_; }
    void iterate(){ EXPECT_TRUE(false); }
    void fit(RecommenderData* recommender_data){ EXPECT_TRUE(recommender_data == &recommender_data_); }
    RecommenderData recommender_data_;
};
class TestPeriodicOfflineLearnerWrapper : public ::testing::Test  {
public:
  PeriodComputerParameters period_computer_parameters;
  PeriodComputer period_computer;
  vector<RecDat> rec_data;
  RecommenderData recommender_data;
  SimpleIterator recommender_data_iterator;
  ExperimentEnvironment experiment_environment;
  DummyOfflineLearner offline_learner1, offline_learner2, offline_learner3;
  PeriodicOfflineLearnerWrapperParameters params;
  TestPeriodicOfflineLearnerWrapper() {
  }
  virtual ~TestPeriodicOfflineLearnerWrapper() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    rec_data.resize(20);
    for(uint i=0;i<rec_data.size();i++){
      rec_data[i].time = i;
    }
    recommender_data.set_rec_data(rec_data);
    EXPECT_TRUE(recommender_data.initialize());
    recommender_data_iterator.set_recommender_data(&recommender_data);
    EXPECT_TRUE(recommender_data_iterator.initialize());
    experiment_environment.set_recommender_data_iterator(&recommender_data_iterator);
    period_computer_parameters.period_mode = "samplenum";
    period_computer_parameters.period_length = 5;
    period_computer_parameters.start_time = 8;
    period_computer.set_parameters(&period_computer_parameters);
    period_computer.set_experiment_environment(&experiment_environment);
    EXPECT_TRUE(period_computer.initialize());
    EXPECT_TRUE(period_computer.self_test());
  }
  virtual void TearDown(){}
};

}


TEST_F(TestPeriodicOfflineLearnerWrapper, test) {
  DummyPeriodicOfflineLearnerWrapper learner(&params);
  learner.add_offline_learner(&offline_learner1);
  learner.add_offline_learner(&offline_learner2);
  learner.add_offline_learner(&offline_learner3);
  learner.set_period_computer(&period_computer);
  EXPECT_TRUE(learner.self_test());
  while(recommender_data_iterator.has_next()){
    RecDat* actual = recommender_data_iterator.next();
    learner.update(actual);
    //TODO test if fit is called
  }
  offline_learner1.no_more_fit=true;
  offline_learner2.no_more_fit=true;
  offline_learner3.no_more_fit=true;
  RecDat extra;
  learner.update(&extra);
  learner.update(&extra);
  learner.update(&extra);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

