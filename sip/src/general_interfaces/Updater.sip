
//enum class UpdaterMessage{
//  %TypeHeaderCode
//  #include "general_interfaces/Updater.h"
//  %End
//  start_of_offline_update, 
//  end_of_offline_update,
//  start_of_offline_iterations, 
//  end_of_offline_iterations,
//  start_of_implicit_update_cycle, 
//  end_of_implicit_update_cycle
//};
class Updater{
  %TypeHeaderCode
  #include "general_interfaces/Updater.h"
  %End
  %Docstring
    Interface for updating :py:class:`alpenglow.cpp.Model` instances or other
    objects of the simulation.
    Objects may implement this interface themselves or have one or more
    associated Updater types.

    Examples:

    * :py:class:`alpenglow.cpp.TransitionProbabilityModel` and :py:class:`alpenglow.cpp.TransitionProbabilityModelUpdater`
    * :py:class:`alpenglow.cpp.PopularityModel` has two updating algorithms:
     * :py:class:`alpenglow.cpp.PopularityModelUpdater`
     * :py:class:`alpenglow.cpp.PopularityTimeframeModelUpdater`
    * :py:class:`alpenglow.cpp.PeriodComputer` implements the Updater interface

    In the online experiment, updaters are organized into a chain.
    See  :doc:`/general/anatomy_of_experiment` for details.

  %End
  public:
    virtual ~Updater();
    virtual void update(RecDat* rec_dat)=0;
  %Docstring
      update(RecDat* rec_dat)

      Updates the associated model or other object of the simulation.

      Parameters
      ----------
      rec_dat : RecDat*
         The newest available sample of the experiment.

  %End
//    virtual void message(UpdaterMessage message);

    bool self_test();
  %Docstring
      Returns true.
  %End
};

