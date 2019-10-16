#include <gtest/gtest.h>
#include "../../main/filters/WhitelistFilter2ModelAdapter.h"


namespace {
  class DummyModel : public Model {
    public:
      double prediction(RecDat* rec_dat) override {
        return rec_dat->time+rec_dat->user+rec_dat->item;
      }
  };
  class DummyWhitelistFilter : public WhitelistFilter {
    public:
      vector<int> get_whitelist(int user) override {
        if (user%2) {
          return vector<int>{0,2,4,6,8};
        } else {
          return vector<int>{1,3,5,7,9};
        }
      }
      bool active(RecDat* rec_dat) override {
        if (rec_dat->item > 10 || rec_dat->item < 0) return false;
        return (rec_dat->item+rec_dat->user) % 2;
      }
  };
  class TestWhitelistFilter2ModelAdapter : public ::testing::Test { 
    public:
      DummyModel model;
      DummyWhitelistFilter filter;
      WhitelistFilter2ModelAdapter adapter;
      TestWhitelistFilter2ModelAdapter(){}
      virtual ~TestWhitelistFilter2ModelAdapter(){}
      void SetUp() override {
        adapter.set_model(&model);
        adapter.set_whitelist_filter(&filter);
        EXPECT_TRUE(adapter.self_test());
      } 
      void TearDown() override {}
  };
}

TEST_F(TestWhitelistFilter2ModelAdapter, prediction){
  RecDat rec_dat;
  rec_dat.time = 10;
  rec_dat.score = 1;
  
  rec_dat.user = 1;
  rec_dat.item = 4; //different parity == active
  double prediction = model.prediction(&rec_dat);
  double adapter_prediction = adapter.prediction(&rec_dat);
  EXPECT_EQ(prediction,adapter_prediction);

  rec_dat.user = 1;
  rec_dat.item = 3; //same parity == active
  adapter_prediction = adapter.prediction(&rec_dat);
  EXPECT_EQ(0,adapter_prediction);
}

TEST_F(TestWhitelistFilter2ModelAdapter, iterator){
  int user = 3;
  RankingScoreIterator* rsi = adapter.get_ranking_score_iterator(user);
  ASSERT_NE(rsi,nullptr);
  EXPECT_EQ(5,rsi->unique_items_num());
  vector<int> scores(10,0);
  int counter = 0;
  while(rsi->has_next()){
    pair<int,double> item_score = rsi->get_next();
    ASSERT_LT(item_score.first, 10);
    scores[item_score.first] = item_score.second;
    counter++;
  }
  EXPECT_EQ(5,counter);
  for(int i=0;i<5;i++){
    EXPECT_EQ(0+user+2*i,scores[2*i]);
    EXPECT_EQ(0,scores[2*i+1]);
  }

  user = 2;
  rsi = adapter.get_ranking_score_iterator(user);
  ASSERT_NE(rsi,nullptr);
  EXPECT_EQ(5,rsi->unique_items_num());
  scores.clear();scores.resize(10);
  counter = 0;
  while(rsi->has_next()){
    pair<int,double> item_score = rsi->get_next();
    ASSERT_LT(item_score.first, 10);
    scores[item_score.first] = item_score.second;
    counter++;
  }
  EXPECT_EQ(5,counter);
  for(int i=0;i<5;i++){
    EXPECT_EQ(0,scores[2*i]);
    EXPECT_EQ(0+user+2*i+1,scores[2*i+1]);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
