
class AvailabilityFilter
: public WhitelistFilter
, public NeedsExperimentEnvironment
{
  %TypeHeaderCode
  #include "filters/AvailabilityFilter.h"
  %End
  %Docstring
    This filter filters the set of available items based on
    (time,itemId,duration) triplets. These have to be preloaded before using
    this filter.

    Sample code
    .. code-block:: python
      :linenos:

      f = rs.AvailabilityFilter()
      f.add_availability(20,1,10) #item 1 is available in the time interval (20,30)
  %End
public:
  bool active(RecDat*);
  std::vector<int> get_whitelist(int user);
  bool self_test();
  void add_availability(double time, int id, int duration);
};

