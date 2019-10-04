#include <vector>
#include <gtest/gtest.h>
#include "../../main/objectives/ObjectiveMSE.h"


namespace {

class TestObjectiveMSE : public ::testing::Test  {
public:
  ObjectiveMSE objective;
  TestObjectiveMSE() {
  }
  virtual ~TestObjectiveMSE() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp() override {
  }
  virtual void TearDown() override {
  }
};

}
TEST_F(TestObjectiveMSE, gradient) {
  RecPred recPred1;
  recPred1.score=1.0;
  recPred1.prediction=0.5;
  EXPECT_EQ(-0.5,objective.get_gradient(&recPred1));
  RecPred recPred2;
  recPred2.score=1.0;
  recPred2.prediction=1.0;
  EXPECT_EQ(0.0,objective.get_gradient(&recPred2));
  RecPred recPred3;
  recPred3.score=0.0;
  recPred3.prediction=0.3;
  EXPECT_EQ(0.3,objective.get_gradient(&recPred3));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

