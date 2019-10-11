#include <vector>
#include <gtest/gtest.h>
#include "../../main/negative_sample_generators/HighPredictionNegativeSampleGenerator.h"

namespace {
class DummyModel : public Model{
  double prediction(RecDat* recDat) override { return recDat->item%7; }
  void add(RecDat* recDat) override {}
};

class TestHighPredictionNegativeSampleGenerator : public ::testing::Test {
  public:
    SpMatrix train_matrix;
    DummyModel model;
    vector<RecDat*> recDats;
    void SetUp() override {
    }
    void TearDown() override {
    }
};

}

TEST_F(TestHighPredictionNegativeSampleGenerator,general) {
  HighPredictionNegativeSampleGeneratorParameters parameters;
  parameters.negative_rate = 3;
  parameters.full_negative_rate = 13;
  parameters.seed = 12345678;
  HighPredictionNegativeSampleGenerator generator(&parameters);
  generator.set_model(&model);
  generator.set_train_matrix(&train_matrix);
  vector<int> items;
  for(int i=0;i<14;i++) items.push_back(i);
  generator.set_items(&items);
  ASSERT_TRUE(generator.self_test());
}

TEST_F(TestHighPredictionNegativeSampleGenerator,general1) {
  HighPredictionNegativeSampleGeneratorParameters parameters;
  parameters.negative_rate = 3;
  parameters.full_negative_rate = 3;
  parameters.seed = 12345678;
  HighPredictionNegativeSampleGenerator generator(&parameters);
  generator.set_model(&model);
  generator.set_train_matrix(&train_matrix);
  vector<int> items;
  for(int i=0;i<16;i++) items.push_back(i);
  generator.set_items(&items);
  ASSERT_TRUE(generator.self_test());

  train_matrix.insert(1,2,1.0);
  train_matrix.insert(1,5,1.0);
  train_matrix.insert(1,6,1.0);
  train_matrix.insert(1,7,1.0);
  train_matrix.insert(1,8,1.0);
  train_matrix.insert(1,9,1.0);
  train_matrix.insert(3,4,1.0);
  train_matrix.insert(3,5,1.0);
  train_matrix.insert(3,7,1.0);
  train_matrix.insert(3,8,1.0);
  train_matrix.insert(3,9,1.0);

  RecDat rec_dat;
  rec_dat.user=2;
  rec_dat.item=15;
  vector<int> item_map;
  item_map.resize(15);
  for(int i=0;i<50000;i++){
    vector<int>* samples = generator.generate(&rec_dat);
    int size = samples->size();
    EXPECT_EQ(3, size);
    for(uint j=0;j<samples->size();j++){
      ASSERT_LT(samples->at(j),15);
      item_map[samples->at(j)]++;
    }
  }
  for(int i=0;i<15;i++){
    EXPECT_LT(item_map[i],11000);
    EXPECT_GT(item_map[i],9000);
  }
}
TEST_F(TestHighPredictionNegativeSampleGenerator,general2) {
  HighPredictionNegativeSampleGeneratorParameters parameters;
  parameters.negative_rate = 4;
  parameters.full_negative_rate = 15;
  parameters.seed = 12345678;
  HighPredictionNegativeSampleGenerator generator(&parameters);
  generator.set_model(&model);
  generator.set_train_matrix(&train_matrix);
  vector<int> items;
  for(int i=0;i<16;i++) items.push_back(i);
  generator.set_items(&items);
  ASSERT_TRUE(generator.self_test());

  train_matrix.insert(1,2,1.0);
  train_matrix.insert(1,5,1.0);
  train_matrix.insert(1,6,1.0);
  train_matrix.insert(1,7,1.0);
  train_matrix.insert(1,8,1.0);
  train_matrix.insert(1,9,1.0);
  train_matrix.insert(3,4,1.0);
  train_matrix.insert(3,5,1.0);
  train_matrix.insert(3,7,1.0);
  train_matrix.insert(3,8,1.0);
  train_matrix.insert(3,9,1.0);

  RecDat rec_dat;
  rec_dat.user=2;
  rec_dat.item=15;
  vector<int> item_map;
  item_map.resize(15);
  for(int i=0;i<10;i++){
    vector<int>* samples = generator.generate(&rec_dat);
    int size = samples->size();
    EXPECT_EQ(4, size);
    for(uint j=0;j<samples->size();j++){
      ASSERT_LT(samples->at(j),15);
      item_map[samples->at(j)]++;
    }
  }
  EXPECT_EQ(10,item_map[5]);
  EXPECT_EQ(10,item_map[6]);
  EXPECT_EQ(10,item_map[12]);
  EXPECT_EQ(10,item_map[13]);
}
int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

