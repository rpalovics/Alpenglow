#include <gtest/gtest.h>
#include <limits>
#include "../../main/offline_evaluators/PrecisionRecallEvaluator.h"
namespace {
class DummyModel : public Model{
 public:
  DummyModel(){}
  void add(RecDat* rec_dat){
  }
  double prediction(RecDat* rec_dat){
    return rec_dat->item%3+0.1*rec_dat->item;
  }
};

class DummyFilter : public ModelFilter{
  public:
    void run(double){}
    vector<pair<int,double>>* get_global_items() override { return &items_; }
    vector<pair<int,double>> items_;
};

class TestPrecisionRecallEvaluator : public ::testing::Test { 
  public:
    DummyModel model_;
    DummyFilter filter_;
    RecommenderData train_data_;
    PrecisionRecallEvaluatorParameters params_;

    TestPrecisionRecallEvaluator(){}
    virtual ~TestPrecisionRecallEvaluator(){}
    virtual void SetUp(){
      vector<RecDat> rec_dats;
      rec_dats.resize(6);
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
      train_data_.set_rec_data(rec_dats);
      train_data_.initialize();

      filter_.items_.push_back(make_pair(0,std::numeric_limits<double>::infinity()));
      filter_.items_.push_back(make_pair(1,std::numeric_limits<double>::infinity()));
      filter_.items_.push_back(make_pair(2,std::numeric_limits<double>::infinity()));
      filter_.items_.push_back(make_pair(3,std::numeric_limits<double>::infinity()));
      filter_.items_.push_back(make_pair(4,std::numeric_limits<double>::infinity()));
      filter_.items_.push_back(make_pair(5,std::numeric_limits<double>::infinity()));

    }
    virtual void TearDown(){
    }
    //RecDat* createRecDat(int user, int item, double time){
    //  RecDat recDat;
    //  recDat.user = user;
    //  recDat.item = item;
    //  recDat.score = 1;
    //  recDat.time = time;
    //  rec_dats_.push_back(recDat);
    //  return &rec_dats_[rec_dats_.size()-1];
    //}
};
}

TEST_F(TestPrecisionRecallEvaluator, general){
  params_.cutoff = 3;
  params_.test_file_name = "";
  params_.test_file_type = "online";
  params_.time = 10;
  PrecisionRecallEvaluator evaluator(&params_);
  EXPECT_FALSE(evaluator.self_test());
  evaluator.set_model(&model_);
  evaluator.set_model_filter(&filter_);
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
