#include <gtest/gtest.h>
#include "../../main/models/factor/FactorModelRankingScoreIterator.h"
#include "../../main/ranking/lemp/FactorsLempContainer.h"

namespace {
class TestFactorModelRankingScoreIterator : public ::testing::Test { 
  public:
    TestFactorModelRankingScoreIterator(){}
    virtual ~TestFactorModelRankingScoreIterator(){}
    virtual void SetUp(){
      FactorsParameters parameters;
      parameters.begin_min = -1;
      parameters.begin_max = 1;
      parameters.dimension = 10;
      parameters.seed = 12345;
      factors = Factors(parameters);
    }
    virtual void TearDown(){}
    Factors factors;
};
}


TEST_F(TestFactorModelRankingScoreIterator, test){
  int itemlimit = 997;
  int bucketsize = 64;

  factors.resize(itemlimit);
  for(int i=0; i<itemlimit; i++){
    factors.init(i);
    factors.set_rand(i);
  }

  FactorsLempContainer container(&factors, bucketsize);

  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, itemlimit-1);
  for(int i=0; i<1000; i++){
    int index = distr(eng);
    factors.set_rand(index);
    container.schedule_update_item(index);
  }
  cout << "scheduled1" << endl;
  int id;
  double score;
  int num = 0;
  FactorModelRankingScoreIterator iterator(*factors.get(0), &container);
  while(iterator.has_next()){
    tie(id,score) = iterator.get_next();
    num++;
  }
  EXPECT_EQ(itemlimit, num);


  num = 0;
  double bound = -10;
  int maxid = 0;
  iterator = FactorModelRankingScoreIterator(*factors.get(557), &container);
  while(iterator.has_next(bound)){
    tie(id,score) = iterator.get_next();
    if(score > bound){
      bound = score;
      maxid = id;
    }
    num++;
  }
  EXPECT_GE(itemlimit, num);
  // cout << maxid << endl;
  EXPECT_TRUE(
    (bound >= Util::scalar_product(factors.get(557),factors.get(557))) ||
    (maxid == 557)
  );

}

TEST_F(TestFactorModelRankingScoreIterator, test2){
  int itemlimit = 997;
  int bucketsize = 64;

  factors.resize(itemlimit);
  for(int i=0; i<itemlimit; i++){
    factors.init(i);
    factors.set_rand(i);
  }

  FactorsLempContainer container(&factors, bucketsize);

  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, itemlimit-1);
  for(int i=0; i<10000; i++){
    int index = distr(eng);
    factors.set_rand(index);
    container.schedule_update_item(index);
    if(i%101 == 0){
      container.buckets_begin();
    }
  }
  cout << "scheduled1" << endl;
  int id;
  double score;
  int num = 0;
  FactorModelRankingScoreIterator iterator(*factors.get(0), &container);
  while(iterator.has_next()){
    tie(id,score) = iterator.get_next();
    num++;
  }
  EXPECT_EQ(itemlimit, num);


  num = 0;
  double bound = -10;
  int maxid = 0;
  iterator = FactorModelRankingScoreIterator(*factors.get(557), &container);
  while(iterator.has_next(bound)){
    tie(id,score) = iterator.get_next();
    if(score > bound){
      bound = score;
      maxid = id;
    }
    num++;
  }
  EXPECT_GE(itemlimit, num);
  // cout << maxid << endl;
  EXPECT_TRUE(
    (bound >= Util::scalar_product(factors.get(557),factors.get(557))) ||
    (maxid == 557)
  );

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
