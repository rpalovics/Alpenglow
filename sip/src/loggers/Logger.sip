
class Logger{
  %TypeHeaderCode
  #include "loggers/Logger.h"
  %End
  %Docstring
    Interface for logging results, simulation or debug info etc.

    In the online experiment, :py:class:`alpenglow.cpp.OnineExperiment` calls
    loggers for each sample and at the end of the experiment. See
    :py:class:`alpenglow.cpp.OnineExperiment` for details.

  %End
  public:
    virtual ~Logger();
    virtual void run(RecDat* rec_dat)=0;
  %Docstring
    run(RecDat* rec_dat)

    Logs results, siumlation info etc. It is not allowed to update the model or other simulation objects at that point.

    In the online experiment, :py:class:`alpenglow.cpp.OnlineExperiment` calls this method.

    Parameters
    ----------
    rec_dat : RecDat*
       The newest available sample of the experiment.
  %End
    bool self_test();
  %Docstring
    Returns true.
  %End
};

