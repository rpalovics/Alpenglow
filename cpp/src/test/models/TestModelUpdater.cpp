#include <gtest/gtest.h>
#include "../../main/models/ModelUpdater.h"

namespace {
class DummyModelGradientUpdater : public ModelGradientUpdater {
  public:
    virtual void update(RecDat* rec_dat, double gradient){}
};

class DummyModelMultiUpdater : public ModelMultiUpdater {
  public:
    virtual void update(vector<RecDat>* rec_dats){}
    //bool self_test(){}
};
class TestModelUpdater : public ::testing::Test { 
  public:
    DummyModelGradientUpdater gradient_updater;
    DummyModelMultiUpdater multi_updater;

    TestModelUpdater(){}
    virtual ~TestModelUpdater(){}
    virtual void SetUp(){}
    virtual void TearDown(){}
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
  EXPECT_TRUE(gradient_updater.self_test());
  UpdaterMessage message = UpdaterMessage::start_of_offline_update;
  gradient_updater.message(message);
  RecDat rec_dat = create_rec_dat(10,10,10);
  gradient_updater.beginning_of_updating_cycle(&rec_dat);
  gradient_updater.end_of_updating_cycle(&rec_dat);
}

TEST_F(TestModelUpdater, multi){
  EXPECT_TRUE(multi_updater.self_test());
  vector<RecDat> rec_dats;
  multi_updater.update(&rec_dats);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
