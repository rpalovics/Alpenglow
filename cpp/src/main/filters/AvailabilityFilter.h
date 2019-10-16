#ifndef AVAILABILITY_FILTER_H
#define AVAILABILITY_FILTER_H

//SIP_AUTOCONVERT

#include "ModelFilter.h"
#include <queue>
#include <set>
#include <gtest/gtest_prod.h>
#include <tuple>
#include <functional>
#include "WhiteListFilter.h"
#include "../general_interfaces/Updater.h"

class AvailabilityFilter
: public WhiteListFilter
, public ModelFilter //DEPRECATED
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
  vector<int> get_white_list(int user) override;
  void run(RecDat* rec_dat) override; //DEPRECATED
  vector<pair<int,double>>* get_global_items() override; //DEPRECATED
  bool self_test(){
    bool OK = ModelFilter::self_test(); //TODO remove
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
  void update(RecDat* rec_dat);
  FRIEND_TEST(TestAvailabilityFilter, test);
};

#endif /* AVAILABILITY_FILTER_H */
