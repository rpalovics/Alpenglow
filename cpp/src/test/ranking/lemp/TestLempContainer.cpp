#include <vector>
#include <set>
#include <limits>
#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include "../../../main/ranking/lemp/FactorsLempContainer.h"
#include "../../../main/utils/Util.h"

namespace {

using namespace std;

class TestLempContainer : public ::testing::Test  {
public:
  TestLempContainer() {}
  virtual ~TestLempContainer() {}
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

TEST_F(TestLempContainer, insert_remove){
  factors.resize(100);
  factors.init(0);

  FactorsLempContainer container(&factors, 30);
  EXPECT_EQ(1,container.buckets_.size());
  EXPECT_EQ(1,(*container.buckets_.begin())->item_lengths_.size());

  factors.init(1);
  container.insert_item(1);
  EXPECT_EQ(1,container.buckets_.size());
  EXPECT_EQ(2,(*container.buckets_.begin())->item_lengths_.size());
}

TEST_F(TestLempContainer, find_closest_bucket){
  FactorsLempContainer container(&factors, 1);

  container.buckets_.clear();
  LempBucket *bucket1 = new LempBucket();
  bucket1->item_lengths_.push_back(1);
  container.buckets_.insert(bucket1);
  LempBucket *bucket2 = new LempBucket();
  bucket2->item_lengths_.push_back(3);
  container.buckets_.insert(bucket2);
  LempBucket *bucket3 = new LempBucket();
  bucket3->item_lengths_.push_back(3);
  container.buckets_.insert(bucket3);
  LempBucket *bucket4 = new LempBucket();
  bucket4->item_lengths_.push_back(2);
  container.buckets_.insert(bucket4);
  LempBucket *bucket5 = new LempBucket();
  bucket5->item_lengths_.push_back(5);
  container.buckets_.insert(bucket5);

  EXPECT_EQ(bucket1, *container.find_closest_bucket(1));
  EXPECT_EQ(bucket1, *container.find_closest_bucket(.5));
  EXPECT_EQ(bucket4, *container.find_closest_bucket(1.2));
  EXPECT_EQ(bucket3, *container.find_closest_bucket(2.99));

  vector<double> ordered_lengths;
  for(auto i : container.buckets_){
    ordered_lengths.push_back(i->get_bucket_max());
  }
  EXPECT_EQ(vector<double>({5,3,3,2,1}), ordered_lengths);
}

TEST_F(TestLempContainer, split_on_insert){
  FactorsLempContainer container(&factors, 50);
  factors.resize(1000);
  for(int i=0; i<1000; i++){
    factors.init(i);
    factors.set_rand(i);
    container.insert_item(i);
  }

  double last_max = std::numeric_limits<double>::max();
  double last_len = std::numeric_limits<double>::max();
  for(auto b : container.buckets_){
    EXPECT_LE(b->get_bucket_max(), last_max);
    last_max = b->get_bucket_max();
    EXPECT_GE(50, b->size());

    for(auto item_id : b->item_ids_){
      double ilen = Util::norm(factors.get(item_id));
      EXPECT_LE(ilen, last_len);
      last_len = ilen;
    }
  }
}

TEST_F(TestLempContainer, update){
  int itemlimit = 997;
  int bucketsize = 64;
  int trials = 100000;

  factors.resize(itemlimit);
  for(int i=0; i<itemlimit; i++){
    factors.init(i);
    factors.set_rand(i);
  }

  FactorsLempContainer container(&factors, bucketsize);
  EXPECT_NE(1, container.buckets_.size());
  int num = 0;
  for(auto b : container.buckets_){
    num += b->size();
  }
  EXPECT_EQ(itemlimit, num);

  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, itemlimit-1);
  for(int i=0; i<trials; i++){
    int index = distr(eng);
    factors.set_rand(index);
    container.update_item(index);
  }

  double last_max = std::numeric_limits<double>::max();
  double last_len = std::numeric_limits<double>::max();
  for(auto b : container.buckets_){
    EXPECT_LE(b->get_bucket_max(), last_max);
    last_max = b->get_bucket_max();
    EXPECT_GE(bucketsize, b->size());

    for(int i=0; i<b->size(); i++){
      double ilen = Util::norm(factors.get(b->item_ids_[i]));
      EXPECT_LE(ilen, last_len);
      EXPECT_EQ(ilen, b->item_lengths_[i]);
      vector<double> itemcopy(*factors.get(b->item_ids_[i]));
      Util::multiply_vector(1/ilen, &itemcopy);
      EXPECT_EQ(itemcopy, b->item_directions_[i]);
      last_len = ilen;
    }
  }
}

TEST_F(TestLempContainer, scheduled_update){
  int itemlimit = 997;
  int bucketsize = 64;
  int trials = 100000;

  factors.resize(itemlimit);
  for(int i=0; i<itemlimit; i++){
    factors.init(i);
    factors.set_rand(i);
  }

  FactorsLempContainer container(&factors, bucketsize);
  EXPECT_NE(1, container.buckets_.size());
  EXPECT_EQ(itemlimit, container.item_buckets_.size());

  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, itemlimit-1);
  for(int i=0; i<trials; i++){
    int index = distr(eng);
    factors.set_rand(index);
    container.schedule_update_item(index);
  }

  container.perform_scheduled_updates();

  double last_max = std::numeric_limits<double>::max();
  double last_len = std::numeric_limits<double>::max();
  for(auto b : container.buckets_){
    EXPECT_LE(b->get_bucket_max(), last_max);
    last_max = b->get_bucket_max();
    EXPECT_GE(bucketsize, b->size());

    for(int i=0; i<b->size(); i++){
      double ilen = Util::norm(factors.get(b->item_ids_[i]));
      EXPECT_LE(ilen, last_len);
      EXPECT_EQ(ilen, b->item_lengths_[i]);
      vector<double> itemcopy(*factors.get(b->item_ids_[i]));
      Util::multiply_vector(1/ilen, &itemcopy);
      EXPECT_EQ(itemcopy, b->item_directions_[i]);
      last_len = ilen;
    }
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}