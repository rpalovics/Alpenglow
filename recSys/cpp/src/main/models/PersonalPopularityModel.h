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
    void add(RecDat * rec_dat);
    double prediction(RecDat * rec_dat);
  private:
    double get_user_best_item_score(int user);
    SpMatrix train_matrix_;
    map<int, int> userBestItems_;

    friend class PersonalPopularityModelUpdater;
    FRIEND_TEST(TestPersonalPopularityModel, update);
};

#endif
