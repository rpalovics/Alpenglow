#ifndef AVAILABILITY_FILTER_H
#define AVAILABILITY_FILTER_H

#include "../recommender_data/RecommenderData.h"
#include "../models/Model.h"
#include "ModelFilter.h"
#include <queue>
#include <set>
#include <gtest/gtest_prod.h>
#include <tuple>

class AvailabilityFilter : ModelFilter {
public:
  virtual void run(RecDat* rec_dat);
  virtual vector<pair<int,double>>* get_global_items();
  virtual vector<pair<int,double>>* get_global_users();
  virtual vector<pair<int,double>>* get_personalized_items(int user){
    return get_global_items();
  }
  virtual vector<pair<int,double>>* get_personalized_users(int item){
    return get_global_users();
  }
  virtual bool active(RecDat*);
  virtual ~AvailabilityFilter(){}
  bool self_test(){
    bool OK = true;
    return OK;
  }
  virtual void addAvailability(double time, int id, int duration);
protected:
  priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<tuple<double,int,int>>> availabilites_;
  priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double,int>>> availability_ends_;
  set<pair<int, double>> available_items_set_;
  vector<pair<int, double>> available_items_;
private:
  FRIEND_TEST(TestAvailabilityFilter, test);
};

#endif
