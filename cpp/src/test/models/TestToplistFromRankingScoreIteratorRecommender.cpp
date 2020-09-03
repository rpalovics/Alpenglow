#include <cstdio>
#include <iostream>
#include <gtest/gtest.h>
#include "../../main/models/TopListRecommender.h"

namespace {
class DummyRankingScoreIterator : public RankingScoreIterator {
public:
  DummyRankingScoreIterator(){
    predictions_.push_back(make_pair(1,4.0));
    predictions_.push_back(make_pair(2,6.0));
    predictions_.push_back(make_pair(3,5.0));
  }
  bool has_next(double lower_bound){
    return counter_<(int)predictions_.size();
  }
  pair<int, double> get_next(){
    return predictions_.at(counter_++);
  }
  int unique_items_num(){
    return predictions_.size();
  }
  vector<pair<int,double>> predictions_;
  int counter_ =0;
};
class DummyRecommender
  : public RankingScoreIteratorProvider
  , public ToplistFromRankingScoreRecommender
  { 
  public:
    RankingScoreIterator* get_ranking_score_iterator(int user) override {
      return &rsi_;
    }
    DummyRankingScoreIterator rsi_;
};
class TestToplistFromRankingScoreRecommender : public ::testing::Test { 
  public:
    TestToplistFromRankingScoreRecommender(){}
    virtual ~TestToplistFromRankingScoreRecommender(){}
    void SetUp() override {}
    void TearDown() override {}
};
} //namespace

TEST_F(TestToplistFromRankingScoreRecommender, test){
  DummyRecommender recommender;
  vector<pair<int,double>> top_list = recommender.get_top_list(1,10,NULL);
  ASSERT_EQ(3,top_list.size());
  EXPECT_EQ(2,top_list[0].first);
  EXPECT_EQ(6.0,top_list[0].second);
  EXPECT_EQ(3,top_list[1].first);
  EXPECT_EQ(5.0,top_list[1].second);
  EXPECT_EQ(1,top_list[2].first);
  EXPECT_EQ(4.0,top_list[2].second);
}
TEST_F(TestToplistFromRankingScoreRecommender, exclude){
  DummyRecommender recommender;
  map<int,double> exclude_set;
  exclude_set[2] = 1.0;
  exclude_set[5] = 1.0;
  vector<pair<int,double>> top_list = recommender.get_top_list(1,10,&exclude_set);
  ASSERT_EQ(2,top_list.size());
  EXPECT_EQ(3,top_list[0].first);
  EXPECT_EQ(5.0,top_list[0].second);
  EXPECT_EQ(1,top_list[1].first);
  EXPECT_EQ(4.0,top_list[1].second);
}
TEST_F(TestToplistFromRankingScoreRecommender, size_limit){
  DummyRecommender recommender;
  vector<pair<int,double>> top_list = recommender.get_top_list(1,1,NULL);
  ASSERT_EQ(1,top_list.size());
  EXPECT_EQ(2,top_list[0].first);
  EXPECT_EQ(6.0,top_list[0].second);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
