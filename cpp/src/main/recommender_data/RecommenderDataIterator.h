#ifndef RECOMMENDER_DATA_ITERATOR_H
#define RECOMMENDER_DATA_ITERATOR_H
/*
 * RecommenderDataIterator.h
 * RecommenderData should be accessed through iterator. An iterator object can shuffle order of rec_dats, but if the data is iterated repeatedly, the iterator must give rec_dats in the same order.
 */


//SIP_AUTOCONVERT

#include "RecommenderData.h"
#include "../utils/SpMatrix.h"

class RecommenderDataIterator : public Initializable {
/**
  Iterator-like interface that serves the dataset as a time series in the online
  experiment. The class also provides random access to the time series.
  Implementations assume that the data is already sorted by time.
*/
public:
  virtual ~RecommenderDataIterator(){}
  void set_recommender_data(RecommenderData* data){ recommender_data_=data; }
  /**
    set_recommender_data(RecommenderData* data)
    Sets the dataset that we iterate on.

    Parameters
    ----------
    data : RecommenderData*
        The dataset.
  */
  virtual bool has_next() const { return counter_ < recommender_data_->size(); } //iterator
  /**
    Returns
    -------
    bool
        Whether the iterator has't reached the end of the time series.
  */
  virtual RecDat* next() = 0; //iterator
  /**
    Advances the iterator and returns a pointer to the following sample.

    Returns
    -------
    RecDat*
        A pointer to the following sample.
  */
  virtual int get_counter() const { return counter_-1; } //get the index of actual sample
  /**
    Returns
    ------
    int
        Index of the actual sample.
  */
  virtual RecDat* get_actual() const = 0; //get actual sample
  /**
    Returns
    ------
    RecDat*
        A pointer to the actual sample.
  */
  virtual RecDat* get(int index) const = 0; //index'th sample from recommender_data
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
  virtual RecDat* get_future(int index) const = 0; //index'th sample from recommender_data, getting sample from future is allowed
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
  virtual int size() const { return recommender_data_->size(); } //number of samples
  /**
    Returns
    -------
    int
        The number of the samples.
  */
  virtual double get_following_timestamp() const = 0; //returns timestamp of the following sample, returns -1 in case of last sample
  /**
    Returns
    -------
    double
        The timestamp of the next sample in the future, i.e., when will the next event happen.
  */
  virtual void restart(){ counter_ = 0; }
  /**
    Restarts the iterator.
  */
  bool self_test(){
    bool ok = true;
    if (recommender_data_ == NULL) ok=false;
    return ok;
  }
  /**
    Tests if the class is set up correctly.
  */
protected:
  bool autocalled_initialize() override {
    return recommender_data_->is_initialized();
  }
  RecommenderData* recommender_data_ = NULL;
  int counter_; //index of next element
};


#endif /* RECOMMENDER_DATA_ITERATOR_H */
