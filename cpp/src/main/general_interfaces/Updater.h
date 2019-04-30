#ifndef UPDATER_H
#define UPDATER_H

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"

enum class UpdaterMessage{
  start_of_offline_update, //iterating once on the samples
  end_of_offline_update,
  start_of_offline_iterations, //iterating multiple times on the samples
  end_of_offline_iterations,
  start_of_implicit_update_cycle, //generated samples will come consequently
  end_of_implicit_update_cycle
};
class Updater{
  /**
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
    //here we should link add_updater function
  */
  public:
    virtual ~Updater(){}
    virtual void update(RecDat* rec_dat)=0;
    /**
      update(RecDat* rec_dat)
      
      Updates the associated model or other object of the simulation.

      Parameters
      ----------
      rec_dat : RecDat*
         The newest available sample of the experiment.

    */
    virtual void message(UpdaterMessage message){}
    ///**
    //  message(UpdaterMessage message)
    //  
    //  TODO describe messaging system

    //  Parameters
    //  ----------
    //  message : UpdaterMessage
    //     The event.
    //*/
    bool self_test(){
      return true;
    }
    /**
      Returns true.
    */
};

#endif /* UPDATER_H */
