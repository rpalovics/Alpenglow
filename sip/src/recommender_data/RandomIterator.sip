
struct RandomIteratorParameters {
  %TypeHeaderCode
  #include "recommender_data/RandomIterator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.RandomIterator`. See documentation there.
  %End
  int seed;
  std::string shuffle_mode;
};
class RandomIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/RandomIterator.h"
  %End
  %Docstring
  RecommenderDataIterator class that completely shuffles data. Note that the
  timestamps won't stay in increasing order, that may cause faults in some
  time-dependent models.

  Sample offline usage: :py:class:`alpenglow.cpp.OfflineIteratingOnlineLearnerWrapper`
  %End
public:
  RandomIterator(RecommenderData* recommender_data, int seed, std::string shuffle_mode);
  RandomIterator(RandomIteratorParameters* params);
  RecDat* next();
  %Docstring
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.next()`
  %End
  void restart();
  %Docstring
    Restarts the iterator. In auto_shuffle mode it also reshuffles the dataset.
  %End
  void shuffle();
  %Docstring
    Reshuffles the dataset.
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
  RecDat* get_actual() const;
  %Docstring
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_actual()`
  %End
  bool self_test();
  %Docstring
    Tests if the class is set up correctly.
  %End
  protected:
  bool autocalled_initialize();
  %Docstring
    See :py:meth:`alpenglow.cpp.Initializable.autocalled_initialize()`.
  %End
};

