#ifndef UPDATER_H
#define UPDATER_H

#include "../recommender_data/RecommenderData.h"

class Updater{
  public:
    virtual void update(RecDat* rec_dat)=0;
    bool self_test(){
      return true;
    }
};

#endif
