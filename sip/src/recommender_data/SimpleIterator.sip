
class SimpleIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/SimpleIterator.h"
  %End
  %Docstring
  This RecommenderDataIterator serves the samples in the original order.
  %End
public:
  SimpleIterator(RecommenderData *rd);
  SimpleIterator();
  RecDat* next();
  %Docstring
    Advances the iterator and returns a pointer to the following sample.

    Returns
    -------
    RecDat*
        A pointer to the following sample.
  %End
  RecDat* get(int index) const;
  %Docstring
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
  %End
  RecDat* get_actual();
  %Docstring
    Returns
    ------
    RecDat*
        A pointer to the actual sample.
  %End
  RecDat* get_future(int index) const;
  %Docstring
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
  %End
  double get_following_timestamp() const;
  %Docstring
    Returns
    -------
    double
        The timestamp of the next sample in the future, i.e., when will the next event happen.
  %End
};

