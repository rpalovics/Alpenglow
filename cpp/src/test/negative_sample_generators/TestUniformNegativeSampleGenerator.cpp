#include <vector>
#include <gtest/gtest.h>
#include "../../main/negative_sample_generators/NegativeSampleGenerator.h"
#include "../../main/negative_sample_generators/UniformNegativeSampleGenerator.h"

namespace {

class TestUniformNegativeSampleGenerator : public ::testing::Test  {
public:
  SpMatrix train_matrix;
  vector<int> items;
  vector<RecDat*> rec_dats;
  TestUniformNegativeSampleGenerator() {
  }
  virtual ~TestUniformNegativeSampleGenerator() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  void SetUp() override {
  }
  void TearDown() override {
  }
  RecDat* create_rec_dat(int user, int item, double score){
      RecDat* rec_dat = new RecDat;
      rec_dat -> user = user;
      rec_dat -> item = item;
      rec_dat -> score = score;
      rec_dats.push_back(rec_dat);
      return rec_dat;
  }
};
} //namespace

TEST_F(TestUniformNegativeSampleGenerator,test) { 
  for(int filter_repeats_val=0;filter_repeats_val<2;filter_repeats_val++){
    UniformNegativeSampleGeneratorParameters parameters;
    parameters.negative_rate = 5;
    parameters.filter_repeats = filter_repeats_val;
    parameters.seed=17339854;
    UniformNegativeSampleGenerator neg_sample_generator(&parameters);
    neg_sample_generator.set_train_matrix(&train_matrix);
    neg_sample_generator.set_items(&items);

    items                   = {1,2,3,4,  6,7,8,9};
    vector<int> user1_items = {1,2,3,4,  6,7};
    vector<int> user2_items = {};
    vector<int> user3_items = {1,2,3,4,  6,7,8,9};

    for ( int item : user1_items ){
      train_matrix.update(1,item,1);
    }
    for ( int item : user2_items ){
      train_matrix.update(2,item,1);
    }
    for ( int item : user3_items ){
      train_matrix.update(3,item,1);
    }
     
    //user1: at least length of 2, contain only 8,9
    RecDat* rec_dat = create_rec_dat(1,10,1);
    vector<int> sample_distrib(10);
    int num_of_generated_samples = 0;
    for(int i=0;i<1000;i++){
      vector<int>* samples = neg_sample_generator.generate(rec_dat);
      num_of_generated_samples+=samples->size();
      EXPECT_GE(samples->size(),2);
      for ( int sample : *samples ) {
        ASSERT_LT(sample,10);
        ASSERT_GE(sample,0);
        sample_distrib[sample]++;
      }
    }
    for(int i=0;i<8;i++){
      EXPECT_EQ(0,sample_distrib[i]);
    }
    for(int i=8;i<10;i++){
      EXPECT_NEAR((double)sample_distrib[i]/num_of_generated_samples,0.5,0.03);
    }

    //user2: length of 5, contain all items
    rec_dat = create_rec_dat(2,10,1);
    sample_distrib.clear();
    sample_distrib.resize(10);
    num_of_generated_samples = 0;
    for(int i=0;i<1000;i++){
      vector<int>* samples = neg_sample_generator.generate(rec_dat);
      num_of_generated_samples+=samples->size();
      EXPECT_EQ(5,samples->size());
      for ( int sample : *samples ) {
        ASSERT_LT(sample,10);
        ASSERT_GE(sample,0);
        sample_distrib[sample]++;
      }
    }
    for(int i : {0,5}){
      EXPECT_EQ(0,sample_distrib[i]);
    }
    for(int i : items){
      EXPECT_NEAR((double)sample_distrib[i]/num_of_generated_samples,1.0/items.size(),0.03);
    }

    //user3: no negative samples
    rec_dat = create_rec_dat(3,10,1);
    for(int i=0;i<10;i++){
      vector<int>* samples = neg_sample_generator.generate(rec_dat);
      EXPECT_EQ(0,samples->size());
    }
  }
}

TEST_F(TestUniformNegativeSampleGenerator,without_replacement) {
  //setting up test object
  UniformNegativeSampleGeneratorParameters parameters;
  parameters.negative_rate = 5;
  parameters.filter_repeats = true;
  parameters.seed=17339854;
  UniformNegativeSampleGenerator neg_sample_generator(&parameters);
  neg_sample_generator.set_train_matrix(&train_matrix);
  neg_sample_generator.set_items(&items);

  //preparing environment
  items                   = {1,2,3,4,  6,7,8,9};
  vector<int> user1_items = {1,2,3,4,  6,7};

  for ( int item : user1_items ){
    train_matrix.update(1,item,1);
  }
   
  //user1: at least length of 2, contain only 8,9
  RecDat* rec_dat = create_rec_dat(1,10,1);
  vector<int> sample_distrib(10);
  int num_of_generated_samples = 0;
  for(int i=0;i<5;i++){
    vector<int>* samples = neg_sample_generator.generate(rec_dat);
    num_of_generated_samples+=samples->size();
    ASSERT_GE(samples->size(),2);
    EXPECT_TRUE(samples->at(0)!=samples->at(1));
  }
}
TEST_F(TestUniformNegativeSampleGenerator,expenv) {
  UniformNegativeSampleGeneratorParameters parameters;
  UniformNegativeSampleGenerator neg_sample_generator(&parameters);
  EXPECT_FALSE(neg_sample_generator.initialize());
  vector<int> items;
  neg_sample_generator.set_items(&items);
  EXPECT_FALSE(neg_sample_generator.initialize());
  ExperimentEnvironment exp_env;
  neg_sample_generator.set_experiment_environment(&exp_env);
  EXPECT_TRUE(neg_sample_generator.initialize());
}
TEST_F(TestUniformNegativeSampleGenerator,destructor) {
  UniformNegativeSampleGeneratorParameters parameters;
  UniformNegativeSampleGenerator* neg_sample_generator 
     = new UniformNegativeSampleGenerator(&parameters);
  delete neg_sample_generator;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

