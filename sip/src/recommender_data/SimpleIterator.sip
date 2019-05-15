
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
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.next()`
  %End
  RecDat* get(int index) const;
  %Docstring
    get(int index)
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get()`
  %End
  RecDat* get_actual() const;
  %Docstring
    See :py:meth:`alpenglow.cpp.RecommenderDataIterator.get_actual()`
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
};

