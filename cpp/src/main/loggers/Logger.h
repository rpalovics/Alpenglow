#ifndef LOGGER
#define LOGGER

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"
using namespace std;

class Logger{
  /**
    Interface for logging results, simulation or debug info etc.

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

    Logs results, siumlation info etc. It is not allowed to update the model or other simulation objects at that point.

    In the online experiment, :py:class:`alpenglow.cpp.OnlineExperiment` calls this method.

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


#endif
