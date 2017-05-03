#ifndef POPULARITY_MODEL
#define POPULARITY_MODEL

#include "Model.h"
#include <vector>
#include <gtest/gtest_prod.h>
#include "../utils/PopContainers.h"

using namespace std;

class PopularityModel : public Model {
  public:
    PopularityModel(){ maxitem = 0;}
    ~PopularityModel(){}
    void add(RecDat * recDat);
    double prediction(RecDat * recDat);
  private:
    PopContainer items;
    int maxitem;
    friend class PopularityModelUpdater;
    FRIEND_TEST(TestPopularityModel, modelsize);
    FRIEND_TEST(TestPopularityModel, update);
    FRIEND_TEST(TestPopularityModel, learn);
};

#endif
