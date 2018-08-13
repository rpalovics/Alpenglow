
struct RandomIteratorParameters{
  %TypeHeaderCode
  #include "recommender_data/RandomIterator.h"
  %End
  int seed;
  bool shuffle;
};

class RandomIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/RandomIterator.h"
  %End
  public:
    RandomIterator(RecommenderData* recommender_data, int seed, bool shuffle);
    RandomIterator(RandomIteratorParameters* params);
    RecDat* get(int index) const ;
    RecDat* get_future(int index) const ;
    double get_following_timestamp() const ;
    RecDat* get_actual();
    void restart();
  private:
    bool autocalled_initialize();
};

