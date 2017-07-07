#ifndef UPDATER_H
#define UPDATER_H

#include "../recommender_data/RecommenderData.h"

class Updater{
  public:
    virtual void update(RecDat* rec_dat)=0;
    virtual void start_of_updating(RecDat* rec_dat){} //called in the offline framework
    virtual void end_of_updating(RecDat* rec_dat){} //called in the offline framework
    bool self_test(){
      return true;
    }
};

#endif
