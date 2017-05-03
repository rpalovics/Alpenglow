#ifndef POPULARITY_UPDATER
#define POPULARITY_UPDATER

#include "ModelUpdater.h"
#include "../models/PopularityModel.h"
#include <vector>
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityModelUpdater : public ModelSimpleUpdater {
  public:
    PopularityModelUpdater(){
      model = NULL;
    }
    void setModel(PopularityModel * model_){model = model_;};
    virtual void update(RecDat * recDat);
    bool selfTest(){
      bool OK = ModelSimpleUpdater::selfTest();
      if(model==NULL){ OK=false; }
      return OK;
    }
  private:
    PopularityModel * model;
};



#endif
