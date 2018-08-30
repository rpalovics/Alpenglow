#include <vector>
#include <gtest/gtest.h>
#include <sstream>
#include "../../main/loggers/InputLogger.h"

namespace {

class TestInputLogger : public ::testing::Test  {
public:
  TestInputLogger() {
  }
  virtual ~TestInputLogger() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){}
  virtual void TearDown(){}
};

}

TEST_F(TestInputLogger, test) {
  std::stringstream out_stringstream;
  InputLoggerParameters params;
  params.output_file = "";
  InputLogger logger(&params);
  EXPECT_FALSE(logger.self_test());
  logger.output_stream_ = &out_stringstream;
  EXPECT_TRUE(logger.self_test());
  RecDat rec_dat;
  rec_dat.time = 0;
  rec_dat.user = 1;
  rec_dat.item = 2;
  rec_dat.id = 0;
  rec_dat.score = 1;
  rec_dat.eval = 1;
  logger.run(&rec_dat);
  rec_dat.time = 1;
  rec_dat.user = 1;
  rec_dat.item = 2;
  rec_dat.id = 2;
  rec_dat.score = 1;
  rec_dat.eval = 0;
  logger.run(&rec_dat);
  rec_dat.time = 1;
  rec_dat.user = 1;
  rec_dat.item = 2;
  rec_dat.id = 3;
  rec_dat.score = 1;
  rec_dat.eval = 0;
  logger.run(&rec_dat);
  rec_dat.time = 1390214039;
  rec_dat.user = 1;
  rec_dat.item = 2;
  rec_dat.id = 3;
  rec_dat.score = 1;
  rec_dat.eval = 0;
  logger.run(&rec_dat);
  std::string out_string = out_stringstream.str();
  EXPECT_EQ("0 1 2 0 1\n1 1 2 2 1\n1 1 2 3 1\n1390214039 1 2 3 1\n", out_string);
}




int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

