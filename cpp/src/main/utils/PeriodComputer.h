#ifndef PERIOD_COMPUTER_H
#define PERIOD_COMPUTER_H

//SIP_AUTOCONVERT

#include <string>
#include "../recommender_data/RecommenderData.h"
#include "../general_interfaces/Updater.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

using namespace std;

struct PeriodComputerParameters {
  string period_mode = "time"; //"samplenum": we treat sample counter as time
  ///**
  //  Available values: `time`, `samplenum`.
  //*/
  int period_length = 86400;
  ///**
  //  The length of the periods (in time or in number of samples).
  //*/
  int start_time = -1; //if time<start_time, end_of_period will be false
  ///**
  //  While time<start_time, end_of_period will be false.
  //*/
};

class PeriodComputer : public Updater, public NeedsExperimentEnvironment, public Initializable {
  /**
    Helper class to compute periods in the time series, e.g., update a model
    weekly or log statistics after every 10000th sample.

    The class has two modes: 

    * period_mode=="time": the time is based on the timestamp queried from recommender_data_iterator.
    * period_mode=="samplenum": the time is based on the number of calls to :py:meth:`update()`.

    The class is notified about the progress of time by calls to the :py:meth:`update()` function.
  */
  public:
    PeriodComputer(PeriodComputerParameters* params){ set_parameters(params); }
    PeriodComputer(){}
    void set_parameters(PeriodComputerParameters* params){
      period_mode_ = params->period_mode;
      period_length_ = params->period_length;
      start_time_ = params->start_time;
    }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){ recommender_data_iterator_ = recommender_data_iterator; }
    bool end_of_period() const; //period num change at the last update
    /**
      True if the current sample is the last in the current period.

      If period_mode==time, the function returns true if the timestamp of the
      next sample falls into the next timeframe.
      If period_mode==samplenum, the function returns true if the next call to
      :py:meth:`update()` will increase the number of the timeframe.

      Returns
      -------
      bool
          True if the current sample is the last in the current period.

    */
    void update(RecDat*);
    /**
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

    */
    int get_period_num() const;
    /**
      Returns the number of the current period, i.e. timestamp/period_length+1.


      If period_mode==time, timestamp is the time field of the current recdat.
      If period_mode==samplenum, timestamp is the number of calls to the
      :py:meth:`update()` function.

      Returns
      ----------
      int
          The index of the current period.

    */
    bool self_test(){
      bool ok = Updater::self_test();
      if (recommender_data_iterator_==NULL) ok=false;
      if (period_mode_ != "time" and period_mode_ != "samplenum") ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if (recommender_data_iterator_==NULL) {
        if (experiment_environment_ == NULL) return false;
        recommender_data_iterator_ = experiment_environment_->get_recommender_data_iterator();
      }
      if(period_mode_=="time"){
        if(!recommender_data_iterator_->is_initialized()) return false;
        timestamp_ = recommender_data_iterator_->get_following_timestamp();
        period_num_ = timestamp_/period_length_;
      }
      return true;
    }
  private:
    const RecommenderDataIterator* recommender_data_iterator_ = NULL;
    //state
    bool end_of_period_ = false;
    int period_num_ = 0; 
    double timestamp_ = 0;
    bool reached_end_already_ = false;
    //parameters TODO const...
    string period_mode_ = "time";
    int period_length_ = 86400;
    int start_time_ = -1;
};

#endif /* PERIOD_COMPUTER_H */
