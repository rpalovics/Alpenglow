#ifndef SIMPLEITERATOR_H_
#define SIMPLEITERATOR_H_
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
    Advances the iterator and returns a pointer to the following sample.
  
    Returns
    -------
    RecDat*
        A pointer to the following sample.
  */
  RecDat* get(int index) const override;
  /**
    get(int index)
    This method provides random access to the past samples of the time series.
    It reaches an error if index is larger than the index of the current sample,
    i.e. if one tries to access data from the future through this function.

    Use :py:meth:`get_counter` to get the index of the newest available sample.
    Use :py:meth:`get_future` to get data from the future.

    Parameters
    ----------
    index : int
        The index of sample to return.
    Returns
    ------
    RecDat*
        A pointer to the sample.
  */
  RecDat* get_actual() override;
  /**
    Returns
    ------
    RecDat*
        A pointer to the actual sample.
  */
  RecDat* get_future(int index) const override;
  /**
    get_future(int index)
    This method provides random access to the complete time series, including
    future.

    Use :py:meth:`get` to safely get data from the past.

    Parameters
    ----------
    index : int
        The index of sample to return.
    Returns
    ------
    RecDat*
        A pointer to the sample.
  */
  double get_following_timestamp() const override;
  /**
    Returns
    -------
    double
        The timestamp of the next sample in the future, i.e., when will the next event happen.
  */
};

#endif
