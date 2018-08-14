
struct RandomIteratorParameters{
  %TypeHeaderCode
  #include "recommender_data/RandomIterator.h"
  %End
  int seed;
  std::string shuffle_mode;
};
class RandomIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "recommender_data/RandomIterator.h"
  %End
  public:
    RandomIterator(RecommenderData* recommender_data, int seed, std::string shuffle_mode);
    RandomIterator(RandomIteratorParameters* params);
    RecDat* next();
    void restart();
    void shuffle();
    RecDat* get_actual();
    RecDat* get(int index) const;
    RecDat* get_future(int index) const;
    double get_following_timestamp() const;
  private:
    bool autocalled_initialize();
};

