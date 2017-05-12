#ifndef POPULARITY_MODEL
#define POPULARITY_MODEL

#include "Model.h"
#include <vector>
#include <gtest/gtest_prod.h>
#include "../utils/PopContainers.h"

using namespace std;

class PopularityModel : public Model {
  public:
    PopularityModel(){ maxitem_ = 0;}
    ~PopularityModel(){}
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
  private:
    PopContainer items_;
    int maxitem_;
    friend class PopularityModelUpdater;
};

#endif
