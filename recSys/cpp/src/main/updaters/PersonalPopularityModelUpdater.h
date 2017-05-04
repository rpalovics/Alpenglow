#ifndef PERSONAL_POPULARITY_UPDATER
#define PERSONAL_POPULATITY_UPDATER

#include "PopularityModelUpdater.h"
#include "../models/PersonalPopularityModel.h"
#include <vector>
#include <gtest/gtest_prod.h>

using namespace std;

class PersonalPopularityModelUpdater : public PopularityModelUpdater {
  public:
    PersonalPopularityModelUpdater(){
      model = NULL;
    }
    void setModel(PersonalPopularityModel * model_){
      PopularityModelUpdater::setModel(model_);
      model = model_;
    };
    virtual void update(RecDat * recDat);
    bool selfTest(){
      bool OK = PopularityModelUpdater::selfTest();
      if(model==NULL){ OK=false; }
      return OK;
    }
  private:
    PersonalPopularityModel * model;
};



#endif
