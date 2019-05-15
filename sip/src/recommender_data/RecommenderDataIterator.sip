
/*
 * RecommenderDataIterator.h
 * RecommenderData should be accessed through iterator. An iterator object can shuffle order of rec_dats, but if the data is iterated repeatedly, the iterator must give rec_dats in the same order.
 */

class RecommenderDataIterator : public Initializable {
  %TypeHeaderCode
  #include "recommender_data/RecommenderDataIterator.h"
  %End
  %Docstring
  Iterator-like interface that serves the dataset as a time series in the online
  experiment. The class also provides random access to the time series.
  Implementations assume that the data is already sorted by time.
  %End
public:
  virtual ~RecommenderDataIterator();
  void set_recommender_data(RecommenderData* data);
  %Docstring
    set_recommender_data(RecommenderData* data)
    Sets the dataset that we iterate on.

    Parameters
    ----------
    data : RecommenderData*
        The dataset.
  %End
  virtual bool has_next() const;
  %Docstring
    Returns
    -------
    bool
        Whether the iterator has't reached the end of the time series.
  %End
  virtual RecDat* next() = 0; 
  %Docstring
    Advances the iterator and returns a pointer to the following sample.

    Returns
    -------
    RecDat*
        A pointer to the following sample.
  %End
  virtual int get_counter() const;
  %Docstring
    Returns
    ------
    int
        Index of the actual sample.
  %End
  virtual RecDat* get_actual() const = 0; 
  %Docstring
    Returns
    ------
    RecDat*
        A pointer to the actual sample.
  %End
  virtual RecDat* get(int index) const = 0; 
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
  virtual RecDat* get_future(int index) const = 0; 
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
  virtual int size() const;
  %Docstring
    Returns
    -------
    int
        The number of the samples.
  %End
  virtual double get_following_timestamp() const = 0; 
  %Docstring
    Returns
    -------
    double
        The timestamp of the next sample in the future, i.e., when will the next event happen.
  %End
  virtual void restart();
  %Docstring
    Restarts the iterator.
  %End
  bool self_test();
  %Docstring
    Tests if the class is set up correctly.
  %End
  protected:
  bool autocalled_initialize();
};

