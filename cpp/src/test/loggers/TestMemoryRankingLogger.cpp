#include <vector>
#include <gtest/gtest.h>
#include <cstdio>
#include "../../main/loggers/MemoryRankingLogger.h"

namespace {

class DummyModel : public Model {
  public:
    double prediction(RecDat* rec_dat){ return 1.0/(rec_dat->item+1); } //smaller item, larger score -> smaller item, smaller rank
};

class TestMemoryRankingLogger : public ::testing::Test  {
public:
  TestMemoryRankingLogger() {
  }
  virtual ~TestMemoryRankingLogger() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  void SetUp() override {

  }
  void TearDown() override {}
};

}

TEST_F(TestMemoryRankingLogger, test) {
  { //the extra bracket makes the removal of temp file easier
    MemoryRankingLoggerParameters params;
    params.out_file="temp_output_file_memory_ranking_logger_test";
    params.memory_log = true;
    params.evaluation_start_time = -1;
    params.top_k = 10;
    params.random_seed = 1234;
    MemoryRankingLogger logger(&params);
    ExperimentEnvironment expenv;
    logger.set_experiment_environment(&expenv);
    DummyModel model;
    logger.set_model(&model);
    RankingLogs log;
    logger.set_ranking_logs(&log);
    EXPECT_TRUE(logger.initialize());
    EXPECT_TRUE(logger.self_test());
  
    RecDat rec_dat;
    rec_dat.user = 1;
    rec_dat.item = 1;
    rec_dat.time = 1;
    rec_dat.id = 1;
    rec_dat.eval = 1;
    rec_dat.score = 1;
    for(int item=1;item<10;item++){
      rec_dat.item = item;
      expenv.update(&rec_dat);
    }
    rec_dat.user = 2;
    for(int i=1;i<10;i++){
      rec_dat.item = i;
      rec_dat.time = 10+i;
      rec_dat.id = 20+i;
      rec_dat.eval = i%2; //true for item=1,3,5,7,9
      logger.run(&rec_dat);
    }
  
    ASSERT_EQ(5,log.logs.size());
    for(int k=0;k<5;k++){
      int i = 2*k+1;
      EXPECT_EQ(10+i,log.logs[k].time);
      EXPECT_EQ(20+i,log.logs[k].id);
      EXPECT_EQ(i-1,log.logs[k].rank);
      EXPECT_EQ(2,log.logs[k].user);
      EXPECT_EQ(i,log.logs[k].item);
      EXPECT_EQ(1,log.logs[k].score);
      EXPECT_EQ(1.0/(i+1),log.logs[k].prediction);
    }
  }
  remove("temp_output_file_memory_ranking_logger_test");
}

TEST_F(TestMemoryRankingLogger, init_test) {
  MemoryRankingLoggerParameters params;
  MemoryRankingLogger logger(&params);
  logger.initialize();
  EXPECT_FALSE(logger.self_test());
}

TEST_F(TestMemoryRankingLogger, destructor) {
  MemoryRankingLoggerParameters params;
  MemoryRankingLogger* logger = new MemoryRankingLogger(&params);
  delete logger;
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

