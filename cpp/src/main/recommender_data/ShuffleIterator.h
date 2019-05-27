#ifndef SHUFFLE_ITERATOR_H
#define SHUFFLE_ITERATOR_H
///*
// * ShuffleIterator.h
// * Egy RecommenderData peldany elemei kozul az egyezo idobelyegueket megkeveri.
// * Ezutan az adatokat mindig ebben a sorrendben szolgalja ki.
// */

//SIP_AUTOCONVERT

#include "RecommenderData.h"
#include <gtest/gtest_prod.h>
#include "RecommenderDataIterator.h"
#include "../utils/Util.h"
#include "../utils/Random.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

struct ShuffleIteratorParameters {
  int seed = 745578;
};

class ShuffleIterator : public RecommenderDataIterator {
public:
  ShuffleIterator(RecommenderData* data, int seed);
  ShuffleIterator(ShuffleIteratorParameters* params){
    seed_ = params->seed;
  }
  RecDat* next() override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.next()`
  */
  RecDat* get(int index) const override;
  /**
    get(int index)
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get()`
  */
  RecDat* get_future(int index) const override;
  /**
    get_future(int index)
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_future()`
  */
  double get_following_timestamp() const override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_following_timestamp()`
  */
  RecDat* get_actual() const override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_actual()`
  */
  bool self_test(){
    bool ok = RecommenderDataIterator::self_test();
    if (seed_==0) ok=false;
    return ok;
  }
protected:
  bool autocalled_initialize() override;
  /**
    See :py:meth:`alpenglow.cpp.Initializable.autocalled_initialize()`
  */
private:
  FRIEND_TEST(TestRecommenderDataIterator, iterator);
  vector<RecDat*> shuffled_data_;
  int seed_;
  bool parent_is_initialized_ = false;
};
#endif /* SHUFFLE_ITERATOR_H */
