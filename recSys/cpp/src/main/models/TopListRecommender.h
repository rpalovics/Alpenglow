#ifndef TOP_LIST_RECOMMENDER_H
#define TOP_LIST_RECOMMENDER_H

class TopListRecommender{
  //id, score
  virtual vector<tuple<int,double>> get_top_list(int user, int k) = 0;
}

#endif;