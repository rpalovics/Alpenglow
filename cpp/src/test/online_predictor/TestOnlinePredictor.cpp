#include <vector>
#include <gtest/gtest.h>
#include "../../main/utils/ToplistCreator.h"
#include "../../main/loggers/OnlinePredictor.h"

namespace {


  class DummyToplistCreator : public ToplistCreator{
    public:
      DummyToplistCreator(ToplistCreatorParameters* params):ToplistCreator(params){counter=0;}
      vector<RecDat>* run(RecDat*){counter++;return &topPredictions;}    
      vector<RecDat> topPredictions;
      int counter;
  };
  class TestOnlinePredictor : public ::testing::Test  {
    public:
      OnlinePredictor* onlinePredictor;
      DummyToplistCreator* predictionCreator;
      vector<RecDat> recDats;

      TestOnlinePredictor() {}
      virtual ~TestOnlinePredictor(){}
      virtual void SetUp(){
        ToplistCreatorParameters params1;
        params1.top_k=0;
        params1.exclude_known=0;
        predictionCreator = new DummyToplistCreator(&params1);

        OnlinePredictorParameters params;
        params.evaluation_start_time = 10;
        params.time_frame = 20;
        onlinePredictor = new OnlinePredictor(&params);
        onlinePredictor->set_prediction_creator(predictionCreator);
      }
      virtual void TearDown(){
        delete predictionCreator;
        delete onlinePredictor;
      }
      RecDat* createRecDat(int user, int item, double score, double time){
        RecDat recDat;
        recDat.user = user;
        recDat.item = item;
        recDat.score = score;
        recDat.time = time;
        recDats.push_back(recDat);
        return &(recDats[recDats.size()-1]);
      }
  };
} //namespace

TEST_F(TestOnlinePredictor, test) {
  //evaluation_start_time=10
  //time_frame=20
  //time_frame bounds: [10, 30), [30, 50), 70, 90 etc.
  EXPECT_EQ(0,predictionCreator->counter);
  EXPECT_EQ(0,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(0,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,0));
  EXPECT_EQ(0,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(0,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,10));
  EXPECT_EQ(0,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(0,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,15));
  EXPECT_EQ(0,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(0,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,20));
  EXPECT_EQ(0,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(0,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,29));
  EXPECT_EQ(0,predictionCreator->counter);
  EXPECT_EQ(0,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(0,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,30));
  EXPECT_EQ(1,predictionCreator->counter);
  EXPECT_EQ(1,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(1,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,30));
  onlinePredictor->run(createRecDat(0,0,0,35));
  onlinePredictor->run(createRecDat(0,0,0,36));
  onlinePredictor->run(createRecDat(0,0,0,49));
  EXPECT_EQ(1,predictionCreator->counter);
  EXPECT_EQ(1,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(1,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,53));
  EXPECT_EQ(2,predictionCreator->counter);
  EXPECT_EQ(2,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(2,onlinePredictor->past_time_frame_);
  onlinePredictor->run(createRecDat(0,0,0,95));
  EXPECT_EQ(3,predictionCreator->counter);
  EXPECT_EQ(4,onlinePredictor->actual_time_frame_);
  EXPECT_EQ(4,onlinePredictor->past_time_frame_);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

