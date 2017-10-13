#ifndef LEMP_CONTAINER_H
#define LEMP_CONTAINER_H

#include "LempBucket.h"
#include "../../utils/Util.h"
#include <map>
#include <utility>
#include <set>
#include <functional>
#include <algorithm>
#include <gtest/gtest_prod.h>

using namespace std;

class LempContainer{
public:
  LempContainer(int bucket_size) : bucket_size_(bucket_size){}
  ~LempContainer(){
    for(auto i : buckets_){
      delete i;
    }
  }
  //lazy update item, only performed when perform_scheduled_updates is called
  void schedule_update_item(int id);

  //number of items in the container
  int size();

  //first bucket, triggers perform_scheduled_updates
  multiset<LempBucket*>::iterator buckets_begin();
  //last bucket
  multiset<LempBucket*>::iterator buckets_end();
protected:
  //actually updates the item from the attached factor
  void update_item(int id);

  //converts the factor to a convenient format
  virtual vector<tuple<int,double,vector<double>>> get_factor_items()=0;
  //retrieves a factor value
  virtual vector<double> get_factor_item(int id)=0;

  //rebuilds the container fully from the given items
  void rebuild_from_items(vector<tuple<int,double,vector<double>>> &items);
  //performs lazy updates
  void perform_scheduled_updates();

  //finds the bucket that the item with the given length shoudl go in
  multiset<LempBucket*>::iterator find_closest_bucket(double length);
  //inserts item, returns the container bucket
  virtual LempBucket* insert_item(int id);
  //removes item, returns the conatiner bucket or NULL if not found
  LempBucket* remove_item(int id);
  //performs split, elimination, or nothing according to bucket size
  void repair_bucket(multiset<LempBucket*>::iterator bucket, double previous_bucket_max);
  //splits bucket into 2
  void split_bucket(multiset<LempBucket*>::iterator bucket);
  //removes bucket and reinserts it's items
  void eliminate_bucket(multiset<LempBucket*>::iterator bucket);

  //ideal bucket size
  int bucket_size_;
  //buckets ordered by max length
  multiset<LempBucket*, LempBucket::less_comparator> buckets_;
  //item id->container bucket map
  map<int, multiset<LempBucket*>::iterator> item_buckets_;
  //items to be updated
  set<int> scheduled_updates_;

  FRIEND_TEST(TestLempContainer, insert_remove);
  FRIEND_TEST(TestLempContainer, find_closest_bucket);
  FRIEND_TEST(TestLempContainer, split_on_insert);
  FRIEND_TEST(TestLempContainer, update);
  FRIEND_TEST(TestLempContainer, scheduled_update);
};

#endif