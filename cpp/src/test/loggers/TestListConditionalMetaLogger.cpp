#include <gtest/gtest.h>
#include "../../main/loggers/ListConditionalMetaLogger.h"

namespace {
class DummyLogger : public Logger {
  public:
    void run(RecDat* rec_dat){
      called_ids_.push_back(rec_dat->id);
    }
    vector<int> called_ids_;
};

class TestListConditionalMetaLogger : public ::testing::Test  {
public:
  TestListConditionalMetaLogger() {
  }
  virtual ~TestListConditionalMetaLogger() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  void SetUp() override {}
  void TearDown() override {}
};

}

TEST_F(TestListConditionalMetaLogger, test) {
  ListConditionalMetaLoggerParameters params;
  params.should_run_vector = {0,1,0,1,0,1};
  ListConditionalMetaLogger logger(&params);
  EXPECT_FALSE(logger.self_test());
  DummyLogger dummy_logger;
  logger.set_logger(&dummy_logger);
  EXPECT_TRUE(logger.self_test());

  for(int id=0;id<6;id++){
    RecDat rec_dat;
    rec_dat.id = id;
    logger.run(&rec_dat);
  }
  ASSERT_EQ(3,dummy_logger.called_ids_.size());
  ASSERT_EQ(1,dummy_logger.called_ids_[0]);
  ASSERT_EQ(3,dummy_logger.called_ids_[1]);
  ASSERT_EQ(5,dummy_logger.called_ids_[2]);
}
TEST_F(TestListConditionalMetaLogger, destructor) {
  ListConditionalMetaLoggerParameters params;
  ListConditionalMetaLogger* logger = new ListConditionalMetaLogger(&params);
  delete logger;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

