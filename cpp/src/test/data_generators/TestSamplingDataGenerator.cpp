#include <gtest/gtest.h>
#include "../../main/data_generators/SamplingDataGenerator.h"
#include "../../main/recommender_data/SimpleIterator.h"
#include <vector>

namespace {

class TestSamplingDataGenerator : public ::testing::Test  {
public:
  vector<RecDat> rec_data;
  RecommenderData recommender_data;
  SimpleIterator recommender_data_iterator;
  ExperimentEnvironment experiment_environment;
  SamplingDataGeneratorParameters params;
  TestSamplingDataGenerator() {
  }
  virtual ~TestSamplingDataGenerator() {
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


TEST_F(TestSamplingDataGenerator, uniform) {
  params.distribution = "uniform";
  params.number_of_samples = 9000;
  SamplingDataGenerator data_generator(&params);
  data_generator.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(data_generator.initialize());
  for(int i=0;i<8;i++) recommender_data_iterator.next();
  RecDat* rec_dat = recommender_data_iterator.next();
  //cerr << "actual: " << *rec_dat << endl;
  RecommenderData* local_recommender_data = data_generator.generate_recommender_data(rec_dat);
  EXPECT_EQ(9000,local_recommender_data->size());
  vector<int> ids(20);
  for(int i=0;i<local_recommender_data->size();i++){
    RecDat* local_rec_dat = local_recommender_data->get(i);
    ASSERT_LT(local_rec_dat->id,20);
    ids[local_rec_dat->id]++;
  }
  for(int i=0;i<9;i++){
    EXPECT_LT(900,ids[i]);
    EXPECT_GT(1100,ids[i]);
  }
  for(int i=9;i<20;i++){
    EXPECT_EQ(0,ids[i]);
  }
}

TEST_F(TestSamplingDataGenerator, linear) {
  params.distribution = "linear";
  params.number_of_samples = 3000;
  SamplingDataGenerator data_generator(&params);
  data_generator.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(data_generator.initialize());
  for(int i=0;i<8;i++) recommender_data_iterator.next();
  RecDat* rec_dat = recommender_data_iterator.next();
  //cerr << "actual: " << *rec_dat << endl;
  RecommenderData* local_recommender_data = data_generator.generate_recommender_data(rec_dat);
  EXPECT_EQ(3000,local_recommender_data->size());
  vector<int> ids(20);
  for(int i=0;i<local_recommender_data->size();i++){
    RecDat* local_rec_dat = local_recommender_data->get(i);
    ASSERT_LT(local_rec_dat->id,20);
    ids[local_rec_dat->id]++;
  }
  for(int i=0;i<8;i++){
    EXPECT_LT(ids[i],ids[i+1]);
  }
  for(int i=9;i<20;i++){
    EXPECT_EQ(0,ids[i]);
  }
}

TEST_F(TestSamplingDataGenerator, geometric) {
  params.distribution = "geometric";
  params.number_of_samples = 900;
  params.geometric_param = 0.4;
  SamplingDataGenerator data_generator(&params);
  data_generator.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(data_generator.initialize());
  for(int i=0;i<8;i++) recommender_data_iterator.next();
  RecDat* rec_dat = recommender_data_iterator.next();
  //cerr << "actual: " << *rec_dat << endl;
  RecommenderData* local_recommender_data = data_generator.generate_recommender_data(rec_dat);
  EXPECT_EQ(900,local_recommender_data->size());
  vector<int> ids(20);
  for(int i=0;i<local_recommender_data->size();i++){
    RecDat* local_rec_dat = local_recommender_data->get(i);
    ASSERT_LT(local_rec_dat->id,20);
    ids[local_rec_dat->id]++;
  }
  for(int i=0;i<8;i++){
    EXPECT_LE(ids[i],ids[i+1]);
  }
  for(int i=9;i<20;i++){
    EXPECT_EQ(0,ids[i]);
  }
}

TEST_F(TestSamplingDataGenerator, arctg) {
  params.distribution = "arctg";
  params.number_of_samples = 9000;
  params.y = 5;
  SamplingDataGenerator data_generator(&params);
  data_generator.set_experiment_environment(&experiment_environment);
  EXPECT_TRUE(data_generator.initialize());
  for(int i=0;i<8;i++) recommender_data_iterator.next();
  RecDat* rec_dat = recommender_data_iterator.next();
  //cerr << "actual: " << *rec_dat << endl;
  RecommenderData* local_recommender_data = data_generator.generate_recommender_data(rec_dat);
  EXPECT_EQ(9000,local_recommender_data->size());
  vector<int> ids(20);
  for(int i=0;i<local_recommender_data->size();i++){
    RecDat* local_rec_dat = local_recommender_data->get(i);
    ASSERT_LT(local_rec_dat->id,20);
    ids[local_rec_dat->id]++;
  }
  for(int i=0;i<8;i++){
    EXPECT_LE(ids[i],ids[i+1]);
  }
  for(int i=9;i<20;i++){
    EXPECT_EQ(0,ids[i]);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

