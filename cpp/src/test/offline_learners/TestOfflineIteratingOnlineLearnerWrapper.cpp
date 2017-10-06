#include <gtest/gtest.h>
#include "../../main/offline_learners/OfflineIteratingOnlineLearnerWrapper.h"
namespace {
class DummyUpdater : public Updater{
 public:
  DummyUpdater(){ counter_=0; messages_.resize(6); }
  void update(RecDat* rec_dat) override {
    counter_++;
  }
  void message(UpdaterMessage message){
    messages_[static_cast<int>(message)]++;
  }
  int counter_;
  vector<int> messages_;
};

class TestOfflineIteratingOnlineLearnerWrapper : public ::testing::Test { 
  public:
    vector<RecDat> rec_data_;
    RecommenderData recommender_data_;
    DummyUpdater early_upd1_, early_upd2_, grad_upd1_, grad_upd2_, upd1_, upd2_;
    OfflineIteratingOnlineLearnerWrapperParameters params_;
    TestOfflineIteratingOnlineLearnerWrapper(){}
    virtual ~TestOfflineIteratingOnlineLearnerWrapper(){}
    virtual void SetUp(){
      rec_data_.resize(3);
      rec_data_[0].user=0;
      rec_data_[1].user=1;
      rec_data_[2].user=2;
      rec_data_[0].item=3;
      rec_data_[1].item=4;
      rec_data_[2].item=5;
      recommender_data_.set_rec_data(rec_data_);
      EXPECT_TRUE(recommender_data_.initialize());
    }
    virtual void TearDown(){
    }
};
}

TEST_F(TestOfflineIteratingOnlineLearnerWrapper, test){
  params_.number_of_iterations=4;
  params_.seed=123456789;
  params_.shuffle=true;
  OfflineIteratingOnlineLearnerWrapper learner(&params_);
  EXPECT_FALSE(learner.self_test());
  learner.add_early_updater(&early_upd1_);
  learner.add_early_updater(&early_upd2_);
  learner.add_iterate_updater(&grad_upd1_);
  learner.add_iterate_updater(&grad_upd2_);
  EXPECT_TRUE(learner.self_test());
  learner.add_updater(&upd1_);
  learner.add_updater(&upd2_);
  learner.fit(&recommender_data_);
  vector<int> simple_messages = {1,1,0,0,0,0};
  vector<int> grad_messages = {0,0,1,1,0,0};
  EXPECT_EQ(3,early_upd1_.counter_);
  EXPECT_EQ(simple_messages,early_upd1_.messages_);
  EXPECT_EQ(3,early_upd2_.counter_);
  EXPECT_EQ(simple_messages,early_upd2_.messages_);
  EXPECT_EQ(12,grad_upd1_.counter_);
  EXPECT_EQ(grad_messages,grad_upd1_.messages_);
  EXPECT_EQ(12,grad_upd2_.counter_);
  EXPECT_EQ(grad_messages,grad_upd2_.messages_);
  EXPECT_EQ(3,upd1_.counter_);
  EXPECT_EQ(simple_messages,upd1_.messages_);
  EXPECT_EQ(3,upd2_.counter_);
  EXPECT_EQ(simple_messages,upd2_.messages_);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
