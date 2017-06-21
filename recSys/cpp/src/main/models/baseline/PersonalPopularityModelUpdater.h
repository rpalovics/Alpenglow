#ifndef PERSONAL_POPULARITY_UPDATER
#define PERSONAL_POPULATITY_UPDATER

#include "PersonalPopularityModel.h"
#include "../ModelUpdater.h"
#include <vector>
#include <gtest/gtest_prod.h>

using namespace std;

class PersonalPopularityModelUpdater : public ModelSimpleUpdater{
  public:
    PersonalPopularityModelUpdater(){
      model_ = NULL;
    }
    void set_model(PersonalPopularityModel* model){
      model_ = model;
    };
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool OK = ModelSimpleUpdater::self_test();
      if(model_==NULL){ OK=false; }
      return OK;
    }
  private:
    PersonalPopularityModel* model_;
};



#endif
