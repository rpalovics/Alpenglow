#ifndef RANKING_SCORE_ITERATOR
#define RANKING_SCORE_ITERATOR

#include <utility>
#include <limits>

class RankingScoreIterator {
public:
  virtual bool has_next(double upper_bound)=0;
  virtual bool has_next(){
    return this->has_next(std::numeric_limits<double>::lowest());
  };
  virtual std::pair<int, double> get_next()=0;
  virtual int unique_items_num()=0;
};

class RankingScoreIteratorProvider {
public:
  virtual RankingScoreIterator* get_ranking_score_iterator(int u)=0;
};

#endif