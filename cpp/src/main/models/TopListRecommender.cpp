#include "TopListRecommender.h"

#include <limits>
#include <queue>

vector<pair<int,double>> TopListRecommender::get_top_list(
    int user, int k, const SpMatrix *exclude){
  return get_top_list(user,k,exclude->get(user));
}
vector<pair<int,double>> ToplistFromRankingScoreRecommender::get_top_list(
    int user, int k, const map<int,double> *exclude_items){
  //reverse toplist as a heap
  auto comparator = [](pair<int,double> const &t1, pair<int,double> const &t2) {
    return get<1>(t1) > get<1>(t2);
  };
  priority_queue<pair<int,double>, vector<pair<int,double>>, decltype(comparator)> toplist(comparator);

  RankingScoreIterator* iterator = get_ranking_score_iterator(user);
  double limit = std::numeric_limits<double>::lowest();
  while(iterator->has_next(limit)){
    auto pair = iterator->get_next();
    if(exclude_items != NULL && exclude_items->find(pair.first) != exclude_items->end()){
      continue;
    }

    toplist.push(pair);

    if((int)toplist.size() > k){
      toplist.pop();
    }
    if((int)toplist.size() == k){
      double current_lowest_toplist_score = get<1>(toplist.top());
      if(current_lowest_toplist_score > limit){
        limit = current_lowest_toplist_score;
      }
    }
  }
  vector<pair<int,double>> toplist_vector(toplist.size());
  for(int i=toplist.size()-1; i>=0; i--){
    toplist_vector[i]=toplist.top();
    toplist.pop();
  }
  return toplist_vector;
}
