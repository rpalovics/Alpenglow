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
  public:
    virtual void update(RecDat* rec_dat)=0;
    virtual void message(UpdaterMessage message){}
    bool self_test(){
      return true;
    }
};

#endif
