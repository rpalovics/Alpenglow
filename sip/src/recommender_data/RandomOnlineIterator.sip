
struct RandomOnlineIteratorParameters{
  %TypeHeaderCode
  #include "recommender_data/RandomOnlineIterator.h"
  %End
  int seed;
};
class RandomOnlineIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/RandomOnlineIterator.h"
  %End
  public:
    RandomOnlineIterator(RandomOnlineIteratorParameters* params);
    ~RandomOnlineIterator();
    RecDat* next();
    RecDat* get_actual();
    RecDat* get(int index) const;
    RecDat* get_future(int index) const;
    double get_following_timestamp() const;
  private:
    bool autocalled_initialize();
};

