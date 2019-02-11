#ifndef LOGGER
#define LOGGER

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"
using namespace std;

class Logger{
  /**
    Interface for loggers.

    Logger instances can be set into
    :py:class:`alpenglow.cpp.OnlineExperiment` using the
    `add_logger()` and the `add_end_logger()` function.
    Loggers set through `add_logger()` are called for each sample, before
    updating the environment and models. Loggers set through `add_end_logger()`
    are called at the end of the experiment.
  */
  public:
    virtual ~Logger(){}
    virtual void run(RecDat* rec_dat)=0;
    /**
    run(RecDat* rec_dat)
    Method called by :py:class:`alpenglow.cpp.OnlineExperiment`.
    */
    bool self_test(){
      bool OK = true;
      return OK;
    }
    /**
    Returns true.
    */
};


#endif
