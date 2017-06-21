#ifndef POPULARITY_MODEL
#define POPULARITY_MODEL

#include <vector>
#include "../Model.h"
#include "../../utils/PopContainers.h"
#include <gtest/gtest_prod.h>

using namespace std;

class PopularityModel : public Model {
  public:
    PopularityModel(){ maxitem_ = 0;}
    ~PopularityModel(){}
    void add(RecDat* rec_dat) override {}
    double prediction(RecDat* rec_dat) override;
  private:
    PopContainer items_;
    int maxitem_;
    friend class PopularityModelUpdater;
};

#endif
