#include <gtest/gtest.h>
#include "../../main/data_generators/TimeframeDataGenerator.h"
#include "../../main/recommender_data/SimpleIterator.h"
#include <vector>

namespace {

class TestTimeframeDataGenerator : public ::testing::Test  {
public:
  vector<RecDat> rec_data;
  RecommenderData recommender_data;
  SimpleIterator recommender_data_iterator;
  ExperimentEnvironment experiment_environment;
  TimeframeDataGeneratorParameters params;
  TestTimeframeDataGenerator() {
  }
  virtual ~TestTimeframeDataGenerator() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    rec_data.resize(20);
    for(uint i=0;i<rec_data.size();i++){
      rec_data[i].time = 2.36*i+(i*i)%3+3;
      rec_data[i].id = i;
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


TEST_F(TestTimeframeDataGenerator, environment_samplenum) {
  params.time_frame_length = 10;
  TimeframeDataGenerator data_generator(&params);
  data_generator.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(data_generator.initialize());
  for(int i=0;i<8;i++) recommender_data_iterator.next();
  RecDat* rec_dat = recommender_data_iterator.next();
  int counter = recommender_data_iterator.get_counter();
  RecommenderData* local_recommender_data = data_generator.generate_recommender_data(rec_dat);
  //TODO test: compute mintime, iterate on recommender_data until mintime, compare data
  double now = rec_dat->time;
  double min_time = now - params.time_frame_length;
  int start_index = 0;
  for(;recommender_data.get(start_index)->time<min_time;start_index++);
  ASSERT_EQ(1,0);//TODO check size
  for(int index=start_index;index<counter;index++){
    EXPECT_EQ(recommender_data.get(index),local_recommender_data->get(index));
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

