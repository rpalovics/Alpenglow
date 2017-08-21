#ifndef LEMP_CONTAINER_H
#define LEMP_CONTAINER_H

#include "LempBucket.h"
#include "../../utils/Factors.h"
#include "../../utils/Util.h"
#include <map>
#include <utility>
#include <set>
#include <functional>
#include <gtest/gtest_prod.h>

using namespace std;

class LempContainer{
public:
  LempContainer(Factors *item_factors, int bucket_size);
  ~LempContainer(){
    for(auto i : buckets_){
      delete i;
    }
  }
  void schedule_update_item(int id);

  int size();

  multiset<LempBucket*>::iterator buckets_begin();
  multiset<LempBucket*>::iterator buckets_end();
protected:
  void update_item(int id);
  vector<tuple<int,double,vector<double>>> factor_to_items(Factors *factor);
  void rebuild_from_items(vector<tuple<int,double,vector<double>>> &items);
  void perform_scheduled_updates();

  multiset<LempBucket*>::iterator find_closest_bucket(double length);
  LempBucket* insert_item(int id);
  LempBucket* remove_item(int id);
  void repair_bucket(multiset<LempBucket*>::iterator bucket, double previous_bucket_max);
  void split_bucket(multiset<LempBucket*>::iterator bucket);
  void eliminate_bucket(multiset<LempBucket*>::iterator bucket);

  int bucket_size_;
  Factors* item_factors_;
  multiset<LempBucket*, LempBucket::less_comparator> buckets_;
  map<int, multiset<LempBucket*>::iterator> item_buckets_;
  set<int> scheduled_updates_;

  FRIEND_TEST(TestLempContainer, insert_remove);
  FRIEND_TEST(TestLempContainer, find_closest_bucket);
  FRIEND_TEST(TestLempContainer, split_on_insert);
  FRIEND_TEST(TestLempContainer, update);
  FRIEND_TEST(TestLempContainer, scheduled_update);
};

#endif