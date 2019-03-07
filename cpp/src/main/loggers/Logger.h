#ifndef LOGGER_H
#define LOGGER_H

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"
using namespace std;

class Logger{
  /**
    Interface for evaluating the model and logging results, statistics, simulation data, debug info etc.

    In the online experiment, :py:class:`alpenglow.cpp.OnineExperiment` calls
    loggers for each sample and at the end of the experiment. See
    :py:class:`alpenglow.cpp.OnineExperiment` for details.
    //here we should link add_logger and add_end_logger functions
  */
  public:
    virtual ~Logger(){}
    virtual void run(RecDat* rec_dat)=0;
    /**
    run(RecDat* rec_dat)

    Evaluates the model and logs results, statistics, simulation data, debug info etc.

    In the online experiment, :py:class:`alpenglow.cpp.OnlineExperiment` calls this method.
    It is not allowed to modify the model or other simulation objects through this function.

    Parameters
    ----------
    rec_dat : RecDat*
       The newest available sample of the experiment.
    */
    bool self_test(){
      bool OK = true;
      return OK;
    }
    /**
    Returns true.
    */
};


#endif /* LOGGER_H */
