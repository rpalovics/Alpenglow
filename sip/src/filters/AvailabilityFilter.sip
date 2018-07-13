
class AvailabilityFilter : public ModelFilter {
  %TypeHeaderCode
  #include "filters/AvailabilityFilter.h"
  %End
  %Docstring
    This is the docstring for AvailabilityFilter. This filter filters the set of available items based on (time,itemId,duration) triplets. These have to be preloaded before 

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
  %End
public:
  virtual void run(RecDat* rec_dat);
  %Docstring
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
  %End
//  virtual std::vector<pair<int,double>>* get_global_items();
  virtual bool active(RecDat*);
  bool self_test();
  void add_availability(double time, int id, int duration);
};

