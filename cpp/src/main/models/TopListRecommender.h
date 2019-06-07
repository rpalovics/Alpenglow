#ifndef TOP_LIST_RECOMMENDER_H
#define TOP_LIST_RECOMMENDER_H

#include <vector>
#include "RankingScoreIterator.h"
#include "../utils/SpMatrix.h"

using namespace std;

class TopListRecommender{
public:
  //id, score
  vector<pair<int,double>> get_top_list(int user, int k, const SpMatrix *exclude);
  virtual vector<pair<int,double>> get_top_list(int user, int k, const map<int,double> *exclude) = 0;
};

class ToplistFromRankingScoreRecommender
: public TopListRecommender,
  public virtual RankingScoreIteratorProvider
{
public:
  vector<pair<int,double>> get_top_list(int user, int k, const map<int,double> *exclude) override;
};

#endif /* TOP_LIST_RECOMMENDER_H */
