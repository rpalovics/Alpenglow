#include <gtest/gtest.h>
#include "../../main/utils/PeriodComputer.h"
#include "../../main/recommender_data/SimpleIterator.h"
#include <vector>

namespace {

class TestPeriodComputer : public ::testing::Test  {
public:
  PeriodComputerParameters params;
  PeriodComputer period_computer;
  vector<RecDat> rec_data;
  RecommenderData recommender_data;
  SimpleIterator recommender_data_iterator;
  ExperimentEnvironment experiment_environment;
  TestPeriodComputer() {
  }
  virtual ~TestPeriodComputer() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    rec_data.resize(20);
    for(uint i=0;i<rec_data.size();i++){
      rec_data[i].time = 2.36*i+(i*i)%3+3;
    }
    recommender_data.set_rec_data(rec_data);
    EXPECT_TRUE(recommender_data.initialize());
    recommender_data_iterator.set_recommender_data(&recommender_data);
    EXPECT_TRUE(recommender_data_iterator.initialize());
    experiment_environment.set_recommender_data_iterator(&recommender_data_iterator);
  }
  virtual void TearDown(){}
};

}


TEST_F(TestPeriodComputer, environment_samplenum) {
  params.period_mode = "samplenum";
  params.period_length = 5;
  params.start_time = 8;
  period_computer.set_parameters(&params);
  period_computer.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(period_computer.initialize());
  EXPECT_TRUE(period_computer.self_test());
  
  for(int i=0;i<4;i++){
    period_computer.update(recommender_data_iterator.next());
    EXPECT_FALSE(period_computer.end_of_period());
    EXPECT_EQ(1,period_computer.get_period_num());
  }

  period_computer.update(recommender_data_iterator.next()); //4
  EXPECT_FALSE(period_computer.end_of_period()); //time<start_time
  EXPECT_EQ(1,period_computer.get_period_num());

  for(int i=0;i<4;i++){
    period_computer.update(recommender_data_iterator.next());
    EXPECT_FALSE(period_computer.end_of_period());
    EXPECT_EQ(2,period_computer.get_period_num());
  }

  period_computer.update(recommender_data_iterator.next()); //9
  EXPECT_TRUE(period_computer.end_of_period());
  EXPECT_EQ(2,period_computer.get_period_num());

  for(int i=0;i<4;i++){
    period_computer.update(recommender_data_iterator.next());
    EXPECT_FALSE(period_computer.end_of_period());
    EXPECT_EQ(3,period_computer.get_period_num());
  }

  period_computer.update(recommender_data_iterator.next()); //14
  EXPECT_TRUE(period_computer.end_of_period());
  EXPECT_EQ(3,period_computer.get_period_num());
  
  while(recommender_data_iterator.has_next()){
    period_computer.update(recommender_data_iterator.next());
  }
  EXPECT_TRUE(period_computer.end_of_period());
  RecDat rec_dat;
  period_computer.update(&rec_dat); //delayed sample
  EXPECT_FALSE(period_computer.end_of_period()); //should not cause repeating end
}

TEST_F(TestPeriodComputer, environment_time) {
  params.period_mode = "time";
  params.period_length = 5;
  params.start_time = 8;
  period_computer.set_parameters(&params);
  period_computer.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(period_computer.initialize());
  EXPECT_TRUE(period_computer.self_test());
  while(recommender_data_iterator.has_next()){
    RecDat* actual = recommender_data_iterator.next();
    period_computer.update(actual);
    double following_timestamp = recommender_data_iterator.get_following_timestamp();
    int period_num = actual->time/params.period_length;
    int next_period_num = following_timestamp/params.period_length;
    EXPECT_FALSE(period_computer.end_of_period()^(period_num!=next_period_num and following_timestamp > params.start_time) and recommender_data_iterator.has_next());
    EXPECT_EQ(period_num+1,period_computer.get_period_num());
  }
  EXPECT_TRUE(period_computer.end_of_period());
  RecDat rec_dat;
  period_computer.update(&rec_dat); //delayed sample
  EXPECT_FALSE(period_computer.end_of_period()); //should not cause repeating end
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

