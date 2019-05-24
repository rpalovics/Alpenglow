#ifndef RANDOM_ITERATOR_H
#define RANDOM_ITERATOR_H

//SIP_AUTOCONVERT

#include <string>
#include <stdexcept>
#include "RecommenderData.h"
#include "RecommenderDataIterator.h"
#include "../utils/Random.h"

struct RandomIteratorParameters {
  int seed = 745578;
  string shuffle_mode = "auto_shuffle";
};
class RandomIterator : public RecommenderDataIterator {
/**
  RecommenderDataIterator class that completely shuffles data. Note that the
  timestamps won't stay in increasing order, that may cause faults in some
  time-dependent models.

  Sample offline usage: :py:class:`alpenglow.cpp.OfflineIteratingOnlineLearnerWrapper`
*/
public:
  RandomIterator(RecommenderData* recommender_data, int seed, string shuffle_mode){ //offline usage
    recommender_data_ = recommender_data;
    random_.set(seed);
    shuffle_mode_ = shuffle_mode;
    bool init_success = initialize();
    if (!init_success){
      throw runtime_error("Initialization was not successful.");
    }
  }
  RandomIterator(RandomIteratorParameters* params){ //online usage
    random_.set(params->seed);
    shuffle_mode_=params->shuffle_mode;
  }
  RecDat* next() override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.next()`
  */
  void restart() override;
  /**
    Restarts the iterator. In auto_shuffle mode it also reshuffles the dataset.
  */
  void shuffle();
  /**
    Reshuffles the dataset.
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
    bool ok = RecommenderDataIterator::self_test() && random_.self_test();
    if (shuffle_mode_ != "auto_shuffle" and shuffle_mode_ != "manual_shuffle") ok=false;
    return ok;
  }
  /**
    Tests if the class is set up correctly.
  */
protected:
  bool autocalled_initialize() override;
  /**
    See :py:meth:`alpenglow.cpp.Initializable.autocalled_initialize()`.
  */
private:
  Random random_;
  vector<RecDat*> shuffled_data_;
  string shuffle_mode_ = "auto_shuffle"; //auto_shuffle or manual_shuffle

  bool parent_is_initialized_ = false;
};


#endif /* RANDOM_ITERATOR_H */
