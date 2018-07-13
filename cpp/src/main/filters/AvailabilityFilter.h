#ifndef AVAILABILITY_FILTER_H
#define AVAILABILITY_FILTER_H

//SIP_AUTOCONVERT

#include "ModelFilter.h"
#include <queue>
#include <set>
#include <gtest/gtest_prod.h>
#include <tuple>
#include <functional>

class AvailabilityFilter : public ModelFilter {
/**
    This is the docstring for AvailabilityFilter. This filter filters the set of available items based on (time,itemId,duration) triplets. These have to be preloaded before using this filter.

    Sample code

    .. code-block:: python
      :emphasize-lines: 3,5

      def some_function():
        interesting = False
        print 'This line is highlighted.'
        print 'This one is not...'
        print '...but this one is.'

    .. code-block:: python
      :linenos:
      
      # this is python code
      f = rs.AvailabilityFilter()
      f.add_availability(10,1,10) #item 1 is available in the time interval (10,20)
*/
public:
  virtual void run(RecDat* rec_dat) override;
/**
      run(rec_dat)
      Summary line.

      Extended description of function.

      Parameters
      ----------
      arg1 : int
          Description of arg1
      arg2 : str
          Description of arg2

      Returns
      -------
      bool
          Description of return value
*/
  virtual vector<pair<int,double>>* get_global_items() override;
  virtual bool active(RecDat*) override;
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
  FRIEND_TEST(TestAvailabilityFilter, test);
};

#endif
