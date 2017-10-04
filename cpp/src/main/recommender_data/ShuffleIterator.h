/*
 * ShuffleIterator.h
 * Egy RecommenderData peldany elemei kozul az egyezo idobelyegueket megkeveri.
 * Ezutan az adatokat mindig ebben a sorrendben szolgalja ki.
 */
#ifndef  SHUFFLE_ITERATOR
#define  SHUFFLE_ITERATOR

#include "RecommenderData.h"
#include <gtest/gtest_prod.h>
#include "RecommenderDataIterator.h"
#include "../utils/Util.h"
#include "../utils/Random.h"
#include <algorithm>

using namespace std;

struct ShuffleIteratorParameters{
  int seed;
};

class ShuffleIterator : public RecommenderDataIterator {
  public:
    ShuffleIterator(RecommenderData* data, int seed);
    ShuffleIterator(ShuffleIteratorParameters* params){
      seed_ = params->seed;
    }
    bool autocalled_initialize() override;
    RecDat* next();
    RecDat* get(int index) const override;
    RecDat* get_future(int index) const override;
    double get_following_timestamp() const override;
    RecDat* get_actual() override;
  private:
    FRIEND_TEST(TestRecommenderDataIterator, iterator);
    vector<RecDat*> shuffled_data_;
    int seed_;
    bool parent_is_initialized_ = false;
};
#endif
