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
    Interface for updaters.

    Updater instances can be set into
    :py:class:`alpenglow.cpp.OnlineExperiment` using the
    `add_updater()` function. The :py:meth:`update` function is called for each sample.
  */
  public:
    virtual ~Updater(){}
    virtual void update(RecDat* rec_dat)=0;
    /**
      update(RecDat* rec_dat)
      Function called by :py:class:`alpenglow.cpp.OnlineExperiment` for each
      sample.
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
