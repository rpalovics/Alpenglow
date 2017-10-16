#include <vector>
#include <utility>
#include <algorithm>
#include <gtest/gtest.h>
#include <cstdlib>
#include <limits>
#include "../../../main/ranking/lemp/LempBucket.h"
#include "../../../main/utils/Util.h"

namespace {

using namespace std;

class TestLempBucket : public ::testing::Test  {
public:
  TestLempBucket() {}
  virtual ~TestLempBucket() {}
  virtual void SetUp(){}
  virtual void TearDown(){}
};

TEST_F(TestLempBucket, insert_remove){
  LempBucket bucket;
  vector<double> length_1 = {1};
  vector<double> length_2 = {2};
  vector<double> length_3 = {3};

  bucket.insert_item(1, &length_1);
  bucket.insert_item(3, &length_3);
  bucket.insert_item(2, &length_2);

  EXPECT_EQ(vector<int>({3,2,1}), bucket.item_ids_);

  bucket.insert_item(4, &length_1);

  EXPECT_EQ(vector<int>({3,2,4,1}), bucket.item_ids_);
  EXPECT_EQ(vector<double>({3,2,1,1}), bucket.item_lengths_);
  EXPECT_EQ(vector<vector<double>>({{1},{1},{1},{1}}), bucket.item_directions_);

  bucket.erase_item(2);

  EXPECT_EQ(vector<int>({3,4,1}), bucket.item_ids_);
  EXPECT_EQ(vector<double>({3,1,1}), bucket.item_lengths_);
  EXPECT_EQ(vector<vector<double>>({{1},{1},{1}}), bucket.item_directions_);

  LempBucket bucket_copy = LempBucket(bucket);
  bucket_copy.erase_item(3);
  EXPECT_TRUE(bucket_copy < bucket);
  EXPECT_FALSE(bucket < bucket);

  LempBucket bucket2;
  vector<double> lens;
  for(int i=0; i<5; i++){
    std::vector<double> v(5);
    std::generate(v.begin(), v.end(), std::rand);
    bucket2.insert_item(i, &v);
    lens.push_back(Util::norm(&v));
  }
  double last_len = std::numeric_limits<double>::max();
  sort(lens.begin(), lens.end(), std::greater<double>());
  for(int i=0; i<bucket2.item_lengths_.size(); i++){
    EXPECT_LE(bucket2.item_lengths_[i], last_len);
    EXPECT_EQ(bucket2.item_lengths_[i], lens[i]);
    last_len = bucket2.item_lengths_[i];
  }}

TEST_F(TestLempBucket, split){
  vector<double> length_1 = {1};
  vector<double> length_2 = {2};
  vector<double> length_3 = {3};
  vector<double> length_4 = {4};
  vector<double> length_5 = {5};
  vector<double> length_6 = {6};

  vector<pair<int, vector<double>>> id_vector_pairs = {
    {1,length_1},
    {2,length_3},
    {3,length_5},
    {4,length_2},
    {5,length_4},
    {6,length_6}
  };

  random_shuffle(id_vector_pairs.begin(), id_vector_pairs.end());

  LempBucket bucket;
  for(auto &it : id_vector_pairs){
    int id;
    vector<double> vector;
    tie(id,vector) = it;
    bucket.insert_item(id, &vector);
  }
  EXPECT_EQ(vector<int>({6,3,5,2,4,1}), bucket.item_ids_);

  LempBucket new_bucket = bucket.split(3);
  EXPECT_EQ(vector<int>({6,3,5}), bucket.item_ids_);
  EXPECT_EQ(vector<int>({2,4,1}), new_bucket.item_ids_);
  EXPECT_EQ(6,bucket.get_bucket_max());
  EXPECT_EQ(3,new_bucket.get_bucket_max());

  LempBucket new_bucket2 = new_bucket.split(2);

  EXPECT_EQ(vector<int>({2}), new_bucket.item_ids_);
  EXPECT_EQ(vector<int>({4,1}), new_bucket2.item_ids_);
  EXPECT_EQ(3,new_bucket.get_bucket_max());
  EXPECT_EQ(2,new_bucket2.get_bucket_max());
}
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}