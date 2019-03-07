#ifndef PERSONAL_POPULARITY_MODEL_UPDATER_H
#define PERSONAL_POPULARITY_MODEL_UPDATER_H

//SIP_AUTOCONVERT

#include "PersonalPopularityModel.h"
#include "../ModelUpdater.h"
#include <vector>
#include <gtest/gtest_prod.h>

using namespace std;

class PersonalPopularityModelUpdater : public Updater{
  public:
    PersonalPopularityModelUpdater(){
      model_ = NULL;
    }
    void set_model(PersonalPopularityModel* model){
      model_ = model;
    };
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool OK = Updater::self_test();
      if(model_==NULL){ OK=false; }
      return OK;
    }
  private:
    PersonalPopularityModel* model_;
};



#endif /* PERSONAL_POPULARITY_MODEL_UPDATER_H */
