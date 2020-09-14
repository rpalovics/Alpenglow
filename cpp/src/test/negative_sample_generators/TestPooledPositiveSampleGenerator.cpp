#include <vector>
#include <gtest/gtest.h>
#include "../../main/negative_sample_generators/PooledPositiveSampleGenerator.h"

#define MAX_ID 100

namespace {
class DummyUpdater : public Updater {
  public:
    DummyUpdater(){
      id_stats.resize(MAX_ID);
    }
    void clear(){
      id_stats.clear();
      id_stats.resize(MAX_ID);
      call_count = 0;
    }
    void update(RecDat* rec_dat){
      id_stats.at(rec_dat->id)++;
      call_count++;
    }
    vector<int> id_stats;
    int call_count = 0;
};
class TestPooledPositiveSampleGenerator : public ::testing::Test  {
public:
  TestPooledPositiveSampleGenerator() {
  }
  virtual ~TestPooledPositiveSampleGenerator() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  void SetUp() override {
  }
  void TearDown() override {
    for (RecDat* rec_dat : rec_dats){
      delete rec_dat;
    }
  }
  vector<RecDat*> rec_dats;
  RecDat* create_rec_dat(int id){
    RecDat* rec_dat = new RecDat;
    rec_dat->id = id;
    rec_dats.push_back(rec_dat);
    return rec_dat;
  }
};
}

TEST_F(TestPooledPositiveSampleGenerator,test) {
  PooledPositiveSampleGeneratorParameters params;
  params.pool_size = 12;
  params.positive_rate = 3;
  PooledPositiveSampleGenerator generator(&params); //correct parameters
  DummyUpdater statistics;
  generator.add_updater(&statistics);
  
  for(int i=0;i<4;i++){
    RecDat* rec_dat = create_rec_dat(i);
    generator.update(rec_dat);
  }
  //pool should be full now
  EXPECT_EQ(12,generator.pool_.size());
  statistics.clear();

  for(int i=4;i<MAX_ID;i++){
    RecDat* rec_dat = create_rec_dat(i);
    generator.update(rec_dat);
  }

  EXPECT_EQ((MAX_ID-4)*3,statistics.call_count);
  for(int i=0;i<MAX_ID;i++){
    EXPECT_LE(statistics.id_stats[i],3);
  }
}

TEST_F(TestPooledPositiveSampleGenerator,selftest) {
  PooledPositiveSampleGeneratorParameters params;

  PooledPositiveSampleGenerator generator1(&params); //default parameters
  EXPECT_FALSE(generator1.self_test());

  params.pool_size = 3000;
  params.positive_rate = 3;
  PooledPositiveSampleGenerator generator2(&params); //correct parameters
  EXPECT_TRUE(generator2.self_test());
}
TEST_F(TestPooledPositiveSampleGenerator,generate) {
  PooledPositiveSampleGeneratorParameters params;
  params.pool_size = 12;
  params.positive_rate = 3;
  PooledPositiveSampleGenerator generator(&params); //correct parameters
  
  RecDat rec_dat;
  EXPECT_ANY_THROW(generator.generate(&rec_dat));
}

TEST_F(TestPooledPositiveSampleGenerator,destructor) { //covers D0 destructor
  PooledPositiveSampleGeneratorParameters params;
  PooledPositiveSampleGenerator* generator =
     new PooledPositiveSampleGenerator(&params);
  delete generator;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

