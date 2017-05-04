#ifndef UPDATER_H
#define UPDATER_H

#include "../recommender_data/RecommenderData.h"

class Updater{
  public:
    virtual void update(RecDat* rec_dat)=0;
    bool selfTest(){
      return true;
    }
};

#endif
