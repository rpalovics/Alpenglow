
struct RandomOnlineIteratorParameters {
  %TypeHeaderCode
  #include "recommender_data/RandomOnlineIterator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.RandomOnlineIterator`. See documentation there.
  %End
  int seed;
};
class RandomOnlineIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/RandomOnlineIterator.h"
  %End
  %Docstring
  This RecommenderDataIterator shuffles the samples keeping the timestamps
  ordered and serves them in a fixed random order. Note that the samples are
  modified, the nth sample of the random order gets the timestamp of the nth
  sample of the original data.
  %End
public:
  RandomOnlineIterator(RandomOnlineIteratorParameters* params);
  ~RandomOnlineIterator();
  RecDat* next();
  %Docstring
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.next()`
  %End
  RecDat* get_actual() const;
  %Docstring
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_actual()`
  %End
  RecDat* get(int index) const;
  %Docstring
    get(int index)
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get()`
  %End
  RecDat* get_future(int index) const;
  %Docstring
    get_future(int index)
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_future()`
  %End
  double get_following_timestamp() const;
  %Docstring
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_following_timestamp()`
  %End
  bool self_test();
  private:
  bool autocalled_initialize();
};

