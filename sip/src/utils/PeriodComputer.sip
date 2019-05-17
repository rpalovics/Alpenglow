
struct PeriodComputerParameters {
  %TypeHeaderCode
  #include "utils/PeriodComputer.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.PeriodComputer`. See documentation there.
  %End
  std::string period_mode; 

  int period_length;

  int start_time; 

};

class PeriodComputer : public Updater, public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "utils/PeriodComputer.h"
  %End
  %Docstring
    Helper class to compute periods in the time series, e.g., update a model
    weekly or log statistics after every 10000th sample.

    The class has two modes: 

    * period_mode=="time": the time is based on the timestamp queried from recommender_data_iterator.
    * period_mode=="samplenum": the time is based on the number of calls to :py:meth:`update()`.

    The class is notified about the progress of time by calls to the :py:meth:`update()` function.
  %End
  public:
    PeriodComputer(PeriodComputerParameters* params);
    PeriodComputer();
    void set_parameters(PeriodComputerParameters* params);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool end_of_period() const; 
  %Docstring
      True if the current sample is the last in the current period.

      If period_mode==time, the function returns true if the timestamp of the
      next sample falls into the next timeframe.
      If period_mode==samplenum, the function returns true if the next call to
      :py:meth:`update()` will increase the number of the timeframe.

      Returns
      -------
      bool
          True if the current sample is the last in the current period.

  %End
    void update(RecDat*);
  %Docstring
      update(RecDat*)
      Notifies the class that time has changed.

      If period_mode==samplenum, the time is increased by 1.
      If period_mode==time, the timestamp of the next sample is queried from
      recommender_data_iterator.
      The parameter value is not used at all.

      Parameters
      ----------
      rec_dat : RecDat*
          The current sample. Not used by the implementation.

  %End
    int get_period_num() const;
  %Docstring
      Returns the number of the current period, i.e. timestamp/period_length+1.

      If period_mode==time, timestamp is the time field of the current recdat.
      If period_mode==samplenum, timestamp is the number of calls to the
      :py:meth:`update()` function.

      Returns
      ----------
      int
          The index of the current period.

  %End
    bool self_test();
  protected:
    bool autocalled_initialize();
};

