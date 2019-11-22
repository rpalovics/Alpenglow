#include <gtest/gtest.h>
#include <limits>
#include "../../main/offline_evaluators/PrecisionRecallEvaluator.h"
namespace {
class DummyModel : public Model{
 public:
  DummyModel(){}
  void add(RecDat* rec_dat) override {
  }
  double prediction(RecDat* rec_dat) override {
    return rec_dat->item%3+0.1*rec_dat->item;
    //toplist: 5 2 4 1 3 0
  }
};

class TestPrecisionRecallEvaluator : public ::testing::Test { 
  public:
    DummyModel model_;
    RecommenderData train_data_;
    PrecisionRecallEvaluatorParameters params_;

    TestPrecisionRecallEvaluator(){}
    virtual ~TestPrecisionRecallEvaluator(){}
    void SetUp() override {
      vector<RecDat> rec_dats;
      rec_dats.resize(7);
      rec_dats[0].user=0;
      rec_dats[0].item=0;
      rec_dats[1].user=0;
      rec_dats[1].item=1;
      rec_dats[2].user=0;
      rec_dats[2].item=2;
      rec_dats[3].user=0;
      rec_dats[3].item=3;
      rec_dats[4].user=3;
      rec_dats[4].item=4;
      rec_dats[5].user=3;
      rec_dats[5].item=4;
      rec_dats[5].user=9;
      rec_dats[5].item=5;
      train_data_.set_rec_data(rec_dats);
      train_data_.initialize();

    }
    void TearDown() override {
    }
};
}

TEST_F(TestPrecisionRecallEvaluator, general){
  params_.cutoff = 3; //top3: 5,2,4
  params_.test_file_name = "";
  params_.test_file_type = "online";
  params_.time = 10;
  PrecisionRecallEvaluator evaluator(&params_);
  EXPECT_FALSE(evaluator.self_test());
  evaluator.set_model(&model_);
  evaluator.set_train_data(&train_data_);
  vector<RecDat> rec_dats;
  rec_dats.resize(11);
  rec_dats[0].user=0;
  rec_dats[0].item=4;
  rec_dats[1].user=0;
  rec_dats[1].item=5;

  rec_dats[2].user=1;
  rec_dats[2].item=0;
  rec_dats[3].user=1;
  rec_dats[3].item=1;
  rec_dats[4].user=1;
  rec_dats[4].item=2;
  rec_dats[5].user=1;
  rec_dats[5].item=3;
  rec_dats[6].user=1;
  rec_dats[6].item=4;
  rec_dats[7].user=1;
  rec_dats[7].item=5;

  rec_dats[8].user=2;
  rec_dats[8].item=1;
  rec_dats[9].user=2;
  rec_dats[9].item=3;
  rec_dats[10].user=2;
  rec_dats[10].item=4;
  evaluator.test_data_.set_rec_data(rec_dats);
  ASSERT_TRUE(evaluator.self_test());

  evaluator.evaluate();

  int true_positive = evaluator.compute_true_positive(0);
  EXPECT_EQ(2, true_positive);

  true_positive = evaluator.compute_true_positive(1);
  EXPECT_EQ(3, true_positive);
  
  true_positive = evaluator.compute_true_positive(2);
  EXPECT_EQ(1, true_positive);
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
