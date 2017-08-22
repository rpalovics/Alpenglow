#ifndef UPDATER_H
#define UPDATER_H

#include "../recommender_data/RecommenderData.h"

enum class UpdaterMessage{
  start_of_fit,
  end_of_fit,
  start_of_implicit_update_cycle,
  end_of_implicit_update_cycle
};
class Updater{
  public:
    virtual void update(RecDat* rec_dat)=0;
    virtual void message(UpdaterMessage message){}
    //virtual void start_of_updating(RecDat* rec_dat){} //called in the offline framework
    //virtual void end_of_updating(RecDat* rec_dat){} //called in the offline framework
    bool self_test(){
      return true;
    }
};

#endif
