#include <vector>
#include <gtest/gtest.h>
#include <sstream>
#include "../../main/loggers/PredictionLogger.h"

namespace {
class DummyToplistCreator : public ToplistCreator {
 public:
   DummyToplistCreator(ToplistCreatorParameters* params):ToplistCreator(params){
     list.resize(3);
     for(uint i=0;i<list.size();i++){
       list[i].user = i;
       list[i].item = i+3;
       list[i].score = i/2.0;
     }
   }
   vector<RecDat>* run(RecDat* rec_dat) override {
     return &list;
   }
   vector<RecDat> list;
};

class TestPredictionLogger : public ::testing::Test  {
public:
  TestPredictionLogger() {
  }
  virtual ~TestPredictionLogger() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  void SetUp() override {}
  void TearDown() override {}
};

}

TEST_F(TestPredictionLogger, test) {
  PredictionLogger logger;
  EXPECT_FALSE(logger.self_test());

  ToplistCreatorParameters params;
  DummyToplistCreator toplist_creator(&params);
  logger.set_prediction_creator(&toplist_creator);
  EXPECT_TRUE(logger.self_test());

  RecDat rec_dat;
  rec_dat.id = 3;
  rec_dat.time = 1000;
  logger.run(&rec_dat);
  rec_dat.id = 5;
  rec_dat.time = 1500;
  logger.run(&rec_dat);

  OnlinePredictions* online_predictions = logger.get_predictions();
  ASSERT_EQ(6,online_predictions->users.size());
  ASSERT_EQ(6,online_predictions->ids.size());
  ASSERT_EQ(6,online_predictions->times.size());
  ASSERT_EQ(6,online_predictions->users.size());
  ASSERT_EQ(6,online_predictions->items.size());
  ASSERT_EQ(6,online_predictions->ranks.size());
  ASSERT_EQ(6,online_predictions->scores.size());
  
  //output from the first run
  int id=3;
  int time=1000;
  for(uint i=0;i<toplist_creator.list.size();i++){
    int j=i;
    EXPECT_EQ(i,online_predictions->users[i]);
    EXPECT_EQ(i+3,online_predictions->items[i]);
    EXPECT_EQ(i/2.0,online_predictions->scores[i]);
    EXPECT_EQ(id,online_predictions->ids[i]);
    EXPECT_EQ(time,online_predictions->times[i]);
    EXPECT_EQ(i+1,online_predictions->ranks[i]);
  }
  //output from the second run
  id=5;
  time=1500;
  for(uint i=0;i<toplist_creator.list.size();i++){
    int j=i+toplist_creator.list.size();
    EXPECT_EQ(i,online_predictions->users[j]);
    EXPECT_EQ(i+3,online_predictions->items[j]);
    EXPECT_EQ(i/2.0,online_predictions->scores[j]);
    EXPECT_EQ(id,online_predictions->ids[j]);
    EXPECT_EQ(time,online_predictions->times[j]);
    EXPECT_EQ(i+1,online_predictions->ranks[j]);
  }
}

TEST_F(TestPredictionLogger, destructor) {
  PredictionLogger* logger = new PredictionLogger;
  delete logger;
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

