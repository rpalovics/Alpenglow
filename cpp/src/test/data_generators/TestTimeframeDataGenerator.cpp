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
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

