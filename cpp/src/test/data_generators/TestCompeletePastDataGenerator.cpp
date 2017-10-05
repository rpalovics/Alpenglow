#include <gtest/gtest.h>
#include "../../main/data_generators/CompletePastDataGenerator.h"
#include "../../main/recommender_data/SimpleIterator.h"
#include <vector>

namespace {

class TestCompletePastDataGenerator : public ::testing::Test  {
public:
  vector<RecDat> rec_data;
  RecommenderData recommender_data;
  SimpleIterator recommender_data_iterator;
  ExperimentEnvironment experiment_environment;
  TestCompletePastDataGenerator() {
  }
  virtual ~TestCompletePastDataGenerator() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    rec_data.resize(20);
    for(uint i=0;i<rec_data.size();i++){
      rec_data[i].time = 2.36*i+(i*i)%3+3;
      //cerr << rec_data[i].time << " ";
      rec_data[i].id = i;
    }
    //cerr << endl;
    recommender_data.set_rec_data(rec_data);
    EXPECT_TRUE(recommender_data.initialize());
    recommender_data_iterator.set_recommender_data(&recommender_data);
    EXPECT_TRUE(recommender_data_iterator.initialize());
    experiment_environment.set_recommender_data_iterator(&recommender_data_iterator);
  }
  virtual void TearDown(){}
};

}


TEST_F(TestCompletePastDataGenerator, environment_samplenum) {
  CompletePastDataGenerator data_generator;
  data_generator.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(data_generator.initialize());
  for(int i=0;i<8;i++) recommender_data_iterator.next();
  RecDat* rec_dat = recommender_data_iterator.next();
  //cerr << "actual: " << *rec_dat << endl;
  RecommenderData* local_recommender_data = data_generator.generate_recommender_data(rec_dat);
  EXPECT_EQ(9,local_recommender_data->size());
  vector<int> ids(20);
  for(int i=0;i<local_recommender_data->size();i++){
    RecDat* local_rec_dat = local_recommender_data->get(i);
    ASSERT_LT(local_rec_dat->id,20);
    ids[local_rec_dat->id]++;
  }
  vector<int> expected_ids = {1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0};
  EXPECT_EQ(expected_ids,ids);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

