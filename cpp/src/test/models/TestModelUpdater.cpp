#include <gtest/gtest.h>
#include "../../main/models/ModelUpdater.h"

namespace {
class DummyModelGradientUpdater : public ModelGradientUpdater {
  public:
    void update(RecDat* rec_dat, double gradient) override {}
    void beginning_of_updating_cycle() override { begin_counter_++; }
    void end_of_updating_cycle() override { end_counter_++; }
    int begin_counter_ = 0;
    int end_counter_ = 0;
};

class DummyModelGradientUpdater2 : public ModelGradientUpdater {
  public:
    void update(RecDat* rec_dat, double gradient) override {}
};

class DummyModelMultiUpdater : public ModelMultiUpdater {
  public:
    void update(vector<RecDat>* rec_dats) override {}
    //bool self_test(){}
};
class TestModelUpdater : public ::testing::Test { 
  public:

    TestModelUpdater(){}
    virtual ~TestModelUpdater(){}
    void SetUp() override {}
    void TearDown() override {}
    RecDat create_rec_dat(int user, int item, double time){
      RecDat rec_dat;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.time = time;
      rec_dat.score = 1;
      return rec_dat;
    }
};
} //namespace

TEST_F(TestModelUpdater, gradient){
  DummyModelGradientUpdater gradient_updater;
  EXPECT_TRUE(gradient_updater.self_test());
  UpdaterMessage message = UpdaterMessage::start_of_implicit_update_cycle;
  gradient_updater.message(message);
  EXPECT_EQ(1,gradient_updater.begin_counter_);
  EXPECT_EQ(0,gradient_updater.end_counter_);
  message = UpdaterMessage::end_of_implicit_update_cycle;
  gradient_updater.message(message);
  EXPECT_EQ(1,gradient_updater.begin_counter_);
  EXPECT_EQ(1,gradient_updater.end_counter_);
}

TEST_F(TestModelUpdater, gradient2){
  DummyModelGradientUpdater2 gradient_updater;
  EXPECT_TRUE(gradient_updater.self_test());
  UpdaterMessage message = UpdaterMessage::start_of_implicit_update_cycle;
  gradient_updater.message(message);
  message = UpdaterMessage::end_of_implicit_update_cycle;
  gradient_updater.message(message);
}

TEST_F(TestModelUpdater, multi){
  DummyModelMultiUpdater multi_updater;
  EXPECT_TRUE(multi_updater.self_test());
  vector<RecDat> rec_dats;
  multi_updater.update(&rec_dats);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
