#include <vector>
#include <gtest/gtest.h>
#include "../../main/objectives/ObjectiveBPR.h"


namespace {

class TestObjectiveBPR : public ::testing::Test  {
public:
  ObjectiveBPR objective;
  TestObjectiveBPR() {
  }
  virtual ~TestObjectiveBPR() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  void SetUp() override {
  }
  void TearDown() override {
    //check(recDat);
  }
};

}
TEST_F(TestObjectiveBPR, gradient) {
  RecPred recPred1, recPred2;
  pair<double,double> gradient;

  recPred1.score=1.0;
  recPred1.prediction=0.5;
  recPred2.score=1.0;
  recPred2.prediction=0.1;
  gradient = objective.get_gradient(&recPred1,&recPred2);
  EXPECT_DOUBLE_EQ(-1*1/(double)(1+exp(recPred1.prediction-recPred2.prediction)),gradient.first);
  EXPECT_DOUBLE_EQ(1/(double)(1+exp(recPred1.prediction-recPred2.prediction)),gradient.second);

  recPred1.score=1.0;
  recPred1.prediction=0.3;
  recPred2.score=1.0;
  recPred2.prediction=0.9;
  gradient = objective.get_gradient(&recPred1,&recPred2);
  EXPECT_DOUBLE_EQ(-1*1/(double)(1+exp(recPred1.prediction-recPred2.prediction)),gradient.first);
  EXPECT_DOUBLE_EQ(1/(double)(1+exp(recPred1.prediction-recPred2.prediction)),gradient.second);

  recPred1.score=1.0;
  recPred1.prediction=0.2;
  recPred2.score=1.0;
  recPred2.prediction=0.2;
  gradient = objective.get_gradient(&recPred1,&recPred2);
  EXPECT_DOUBLE_EQ(-1*1/(double)(1+exp(recPred1.prediction-recPred2.prediction)),gradient.first);
  EXPECT_DOUBLE_EQ(1/(double)(1+exp(recPred1.prediction-recPred2.prediction)),gradient.second);


}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

