#ifndef AVAILABILITY_FILTER_H
#define AVAILABILITY_FILTER_H

#include "ModelFilter.h"
#include <queue>
#include <set>
#include <gtest/gtest_prod.h>
#include <tuple>

class AvailabilityFilter : ModelFilter {
public:
  virtual void run(RecDat* rec_dat) override;
  virtual vector<pair<int,double>>* get_global_items() override;
  virtual vector<pair<int,double>>* get_global_users() override;
  virtual bool active(RecDat*) override;
  virtual ~AvailabilityFilter(){}
  bool self_test(){
    bool OK = ModelFilter::self_test();
    return OK;
  }
  void add_availability(double time, int id, int duration);
protected:
  priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<tuple<double,int,int>>> availabilites_;
  priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double,int>>> availability_ends_;
  set<pair<int, double>> available_items_set_;
  vector<pair<int, double>> available_items_;
private:
  FRIEND_TEST(TestAvailabilityFilter, test);
};

#endif
