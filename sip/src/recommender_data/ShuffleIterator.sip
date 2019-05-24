
struct ShuffleIteratorParameters {
  %TypeHeaderCode
  #include "recommender_data/ShuffleIterator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.ShuffleIterator`. See documentation there.
  %End
  int seed;
};

class ShuffleIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/ShuffleIterator.h"
  %End
public:
  ShuffleIterator(RecommenderData* data, int seed);
  ShuffleIterator(ShuffleIteratorParameters* params);
  RecDat* next();
  %Docstring
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.next()`
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
  protected:
  bool autocalled_initialize();
  %Docstring
    See :py:meth:`alpenglow.cpp.Initializable.autocalled_initialize()`
  %End
};

