#ifndef AVAILABILITY_FILTER_H
#define AVAILABILITY_FILTER_H

//SIP_AUTOCONVERT

#include <queue>
#include <set>
#include <gtest/gtest_prod.h>
#include <tuple>
#include <functional>
#include "WhitelistFilter.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"

class AvailabilityFilter
: public WhitelistFilter
, public NeedsExperimentEnvironment
{
/**
    This filter filters the set of available items based on
    (time,itemId,duration) triplets. These have to be preloaded before using
    this filter.

    Sample code

    .. code-block:: python
        :linenos:

        f = rs.AvailabilityFilter()
        f.add_availability(20,1,10) #item 1 is available in the time interval (20,30)

*/
public:
  bool active(RecDat*) override;
  vector<int> get_whitelist(int user) override;
  bool self_test(){
    bool OK = true;
    return OK;
  }
  void add_availability(double time, int id, int duration);
protected:
  priority_queue<tuple<double, int, int>,
                 vector<tuple<double, int, int>>,
                 greater<tuple<double,int,int>>> availabilites_;
  priority_queue<pair<double, int>,
                 vector<pair<double, int>>,
                 greater<pair<double,int>>> availability_ends_;
  set<int> available_items_set_;
  vector<int> available_items_;
private:
  double time_ = -1;
  void update();
  FRIEND_TEST(TestAvailabilityFilter, test);
};

#endif /* AVAILABILITY_FILTER_H */
