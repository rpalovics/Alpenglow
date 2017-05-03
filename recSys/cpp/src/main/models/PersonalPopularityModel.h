#ifndef PERSONAL_POPULARITY_MODEL
#define PERSONAL_POPULARITY_MODEL

#include "PopularityModel.h"
#include <vector>
#include <map>
#include <gtest/gtest_prod.h>
#include "../utils/PopContainers.h"
#include "../recommender_data/RecommenderData.h"

using namespace std;

class PersonalPopularityModel : public PopularityModel {
  public:
    PersonalPopularityModel(){}
    ~PersonalPopularityModel(){}
    void add(RecDat * recDat);
    double prediction(RecDat * recDat);
  private:
    double getUserBestItemScore(int user);
    SpMatrix trainMatrix_;
    map<int, int> userBestItems_;

    friend class PersonalPopularityModelUpdater;
    FRIEND_TEST(TestPersonalPopularityModel, update);
};

#endif
