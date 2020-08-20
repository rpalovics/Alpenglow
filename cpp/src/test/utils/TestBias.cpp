#include <gtest/gtest.h>
#include "../../main/utils/Bias.h"


namespace {

class TestBias : public ::testing::Test  {
public:
  TestBias() { }
  virtual ~TestBias() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  void SetUp() override { }
  void TearDown() override { }
};

}
TEST_F(TestBias, test) {
  Bias bias;
  bias.init(2);
  bias.update(2,0.3);
  EXPECT_EQ(0.3,bias.get(2));
  bias.init(3);
  EXPECT_EQ(0,bias.get(3));
  bias.update(3,0.2);
  EXPECT_EQ(0.2,bias.get(3));
  bias.clear();
  EXPECT_EQ(0,bias.get(2));
  EXPECT_EQ(0,bias.get(3));
}

TEST_F(TestBias, destructor){
  Bias* bias;
  bias = new Bias;
  delete bias;
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

