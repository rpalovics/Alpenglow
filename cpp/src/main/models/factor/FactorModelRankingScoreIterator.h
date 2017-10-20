#ifndef FACTOR_MODEL_RANKING_ITERATOR_H
#define FACTOR_MODEL_RANKING_ITERATOR_H

#include "../RankingScoreIterator.h"
#include "../../ranking/lemp/LempContainer.h"
#include <utility>

using namespace std;

class FactorModelRankingScoreIterator : public RankingScoreIterator{
public:
  FactorModelRankingScoreIterator(vector<double> user_factor, LempContainer *container)
  : user_factor_(user_factor),
    container_(container)
  {
    next_bucket_ = container_->buckets_begin();
    user_factor_norm_ = Util::norm(&user_factor);
  }

  FactorModelRankingScoreIterator(){};

  virtual bool has_next(double upper_bound);
  using RankingScoreIterator::has_next;
  virtual pair<int, double> get_next();
  virtual int unique_items_num();
protected:
  double user_factor_norm_;
  vector<double> user_factor_;
  LempContainer *container_;
  multiset<LempBucket*>::iterator next_bucket_;
  vector<pair<int,double>> current_scores_;
};

#endif