struct ShuffleIteratorParameters{
  %TypeHeaderCode
  #include "recommender_data/ShuffleIterator.h"
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
    RecDat* get(int index) const;
    RecDat* get_actual();
    RecDat* get_future(int index) const;
    double get_following_timestamp() const;
  protected:
    bool autocalled_initialize();
};
