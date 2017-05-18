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
    void set_model(PersonalPopularityModel * model_){
      PopularityModelUpdater::set_model(model_);
      model = model_;
    };
    virtual void update(RecDat * rec_dat);
    bool self_test(){
      bool OK = PopularityModelUpdater::self_test();
      if(model==NULL){ OK=false; }
      return OK;
    }
  private:
    PersonalPopularityModel * model;
};



#endif
