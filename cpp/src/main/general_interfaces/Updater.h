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
    Updater of an associated :py:class:`alpenglow.cpp.Model` instance or other
    object of the simulation.

    :py:class:`alpenglow.cpp.OnlineExperiment` notifies objects about the
    progress of simulation time through the :py:meth:`update` function of this
    interface.
    :py:class:`alpenglow.cpp.OnlineExperiment` accepts `Updater` instances
    through its `add_updater()` function. During the online experiment,
    it calls the :py:meth:`update()` function for each sample.
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
    bool self_test(){
      return true;
    }
    /**
      Returns true.
    */
};

#endif
