#include <vector>
#include <gtest/gtest.h>
#include "../../main/online_learners/LearnerPeriodicDelayedWrapper.h"

namespace {
  class DummyLearner : public Updater {
    public:
      DummyLearner(){}
      ~DummyLearner(){}
      void learn(RecDat* recdat){
        learned_recdats.push_back(recdat);
      }
      void update(RecDat* recdat){
        learn(recdat);
      }
      vector<RecDat*> learned_recdats;
  };

  class TestLearnerPeriodicDelayedWrapper : public ::testing::Test  {
    public:
      vector<RecDat*> rec_dats;
      TestLearnerPeriodicDelayedWrapper(){}
      virtual ~TestLearnerPeriodicDelayedWrapper(){}
      void SetUp() override {
      }
      void TearDown() override {
        for(auto r : rec_dats)
          delete r;
      }
      RecDat* create_rec_dat(int user, int item, double time, double score){
        RecDat* rec_dat = new RecDat();
        rec_dat->user = user;
        rec_dat->item = item;
        rec_dat->time = time;
        rec_dat->score = score;
        rec_dats.push_back(rec_dat);
        return rec_dat;
      }
  };
}

TEST_F(TestLearnerPeriodicDelayedWrapper, test_wrapper){
  auto params = new LearnerPeriodicDelayedWrapperParameters();
  params->delay = -1;
  params->period = -1;
  LearnerPeriodicDelayedWrapper wrapper(params);
  auto wrapped_learner = new DummyLearner();
  wrapper.set_wrapped_learner(wrapped_learner);


  wrapper.update(create_rec_dat(1,1,0,1));
  EXPECT_EQ(1, wrapped_learner->learned_recdats.size());
  wrapper.update(create_rec_dat(1,1,0,5));
  EXPECT_EQ(2, wrapped_learner->learned_recdats.size());
  EXPECT_EQ(5, wrapped_learner->learned_recdats[1]->score);


}


TEST_F(TestLearnerPeriodicDelayedWrapper, test_delay){
  auto params = new LearnerPeriodicDelayedWrapperParameters();
  params->delay = 10;
  params->period = -1;
  LearnerPeriodicDelayedWrapper wrapper(params);
  auto wrapped_learner = new DummyLearner();
  wrapper.set_wrapped_learner(wrapped_learner);


  wrapper.update(create_rec_dat(1,1,0,1));
  wrapper.update(create_rec_dat(1,1,1,5));
  EXPECT_EQ(0, wrapped_learner->learned_recdats.size());
  wrapper.update(create_rec_dat(1,1,15,6));
  EXPECT_EQ(2, wrapped_learner->learned_recdats.size());
  EXPECT_EQ(5, wrapped_learner->learned_recdats[1]->score);


  wrapper.update(create_rec_dat(1,1,17,7));
  wrapper.update(create_rec_dat(1,1,19,8));
  EXPECT_EQ(2, wrapped_learner->learned_recdats.size());

  wrapper.update(create_rec_dat(1,1,26,9));
  EXPECT_EQ(3, wrapped_learner->learned_recdats.size());
  EXPECT_EQ(6, wrapped_learner->learned_recdats[2]->score);
}

TEST_F(TestLearnerPeriodicDelayedWrapper, test_period){
  auto params = new LearnerPeriodicDelayedWrapperParameters();
  params->delay = -1;
  params->period = 10;
  LearnerPeriodicDelayedWrapper wrapper(params);
  auto wrapped_learner = new DummyLearner();
  wrapper.set_wrapped_learner(wrapped_learner);


  wrapper.update(create_rec_dat(1,1,4,1));
  wrapper.update(create_rec_dat(1,1,7,2));
  wrapper.update(create_rec_dat(1,1,9,3));
  wrapper.update(create_rec_dat(1,1,10,4));
  EXPECT_EQ(0, wrapped_learner->learned_recdats.size());
  wrapper.update(create_rec_dat(1,1,14,5));
  EXPECT_EQ(4, wrapped_learner->learned_recdats.size());
  EXPECT_EQ(4, wrapped_learner->learned_recdats[3]->score);
  wrapper.update(create_rec_dat(1,1,18,6));
  wrapper.update(create_rec_dat(1,1,22,7));
  EXPECT_EQ(4, wrapped_learner->learned_recdats.size());
  wrapper.update(create_rec_dat(1,1,30,8));
  EXPECT_EQ(7, wrapped_learner->learned_recdats.size());
  EXPECT_EQ(4, wrapped_learner->learned_recdats[3]->score);
  EXPECT_EQ(7, wrapped_learner->learned_recdats[6]->score);
}

TEST_F(TestLearnerPeriodicDelayedWrapper, test_period_delay){
  auto params = new LearnerPeriodicDelayedWrapperParameters();
  params->delay = 5;
  params->period = 10;
  LearnerPeriodicDelayedWrapper wrapper(params);
  auto wrapped_learner = new DummyLearner();
  wrapper.set_wrapped_learner(wrapped_learner);


  wrapper.update(create_rec_dat(1,1,4,1));
  wrapper.update(create_rec_dat(1,1,7,2));
  wrapper.update(create_rec_dat(1,1,9,3));
  wrapper.update(create_rec_dat(1,1,10,4));
  EXPECT_EQ(0, wrapped_learner->learned_recdats.size());
  wrapper.update(create_rec_dat(1,1,14,5));
  EXPECT_EQ(0, wrapped_learner->learned_recdats.size());
  wrapper.update(create_rec_dat(1,1,19,6));
  EXPECT_EQ(4, wrapped_learner->learned_recdats.size());
  EXPECT_EQ(4, wrapped_learner->learned_recdats[3]->score);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
