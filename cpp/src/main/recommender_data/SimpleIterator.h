#ifndef SIMPLE_ITERATOR_H
#define SIMPLE_ITERATOR_H
#include <stdexcept>
#include "RecommenderDataIterator.h"

//SIP_AUTOCONVERT
class SimpleIterator : public RecommenderDataIterator {
/**
  This RecommenderDataIterator serves the samples in the original order.
*/
public:
  SimpleIterator(RecommenderData *rd){
    counter_=0;
    recommender_data_=rd;
  }
  SimpleIterator(){
    counter_=0;
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
  RecDat* get_actual() const override;
  /**
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_actual()`
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
};

#endif /* SIMPLE_ITERATOR_H */
