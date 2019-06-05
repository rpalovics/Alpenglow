#ifndef NEAREST_NEIGHBOR_MODEL_RANKING_SCORE_ITERATOR_H
#define NEAREST_NEIGHBOR_MODEL_RANKING_SCORE_ITERATOR_H

//SIP_AUTOCONVERT

#include <iostream>
#include <gtest/gtest_prod.h>
#include "../Model.h"
#include "../../utils/UserHistory.h"
#include "../../filters/ModelFilter.h"
#include "../../recommender_data/macros.h"
#include "../RankingScoreIterator.h"

class NearestNeighborModelRankingScoreIterator : public RankingScoreIterator {
public:
  bool has_next(double bound) override;
  pair<int, double> get_next() override;
  void set_up(vector<SCORE>& predictions, vector<ITEM>& active_items);
  void reinit(){counter_=0;}
  int unique_items_num() override { throw runtime_error("Not implemented. Should not be called by RankComputer as all scores are nonnegatives."); }
private:
  void clear(){counter_=0;current_scores_.clear();}
  vector<pair<int,double>> current_scores_;
  int counter_;
};
struct NearestNeighborModelParameters {
  double gamma=-1;
  string norm="";
  string direction="";
  double gamma_threshold=-1;
  int num_of_neighbors=-1;
};
class NearestNeighborModel
  : public Model
  , virtual public RankingScoreIteratorProvider
{
public:
  NearestNeighborModel(NearestNeighborModelParameters* params):
    gamma_(params->gamma),
    gamma_threshold_(params->gamma_threshold),
    num_of_neighbors_(params->num_of_neighbors),
    norm_type_(params->norm==""?"num":params->norm),
    direction_(params->direction==""?"forward":params->direction)
  {}
  double prediction(RecDat* rec_dat) override;
  RankingScoreIterator* get_ranking_score_iterator(int user) override;
  bool self_test(){
    bool ok = Model::self_test();
    if(gamma_==-1){ ok=false; cerr << "NearestNeighborModel::gamma is not set properly." << endl; }
    if(gamma_threshold_<0){ ok=false; cerr << "NearestNeighborModel::gamma_threshold is not set properly." << endl; }
    if(num_of_neighbors_<0){ ok=false; cerr << "NearestNeighborModel::num_of_neighbors is not set properly." << endl; }
    if(!(norm_type_=="off" or norm_type_=="sum" or norm_type_=="num")){ ok = false; cerr << "NearestNeighborModel::norm is not set properly." << endl; }
    if(norm_type_=="sum"){ cerr << "WARNING: NearestNeighborModel::norm_type=sum is not implemented yet." << endl; }
    if(!(direction_=="forward" or direction_=="both")){ ok = false; cerr << "NearestNeighborModel::direction is not set properly." << endl; }
    return ok;
  }
  //vector<pair<int,double>>* get_personalized_items(int user) override; //deprecated
private:
  //params
  const double gamma_;
  const double gamma_threshold_;
  const int num_of_neighbors_;
  const string norm_type_;
  const string direction_;

  //state
  UserHistory user_histories_;

  //cache
  vector<vector<pair<ITEM,double>>> similarities_;
  vector<SCORE> active_user_predictions_;
  vector<ITEM> nonzero_items_;
  int active_user_ = -1;
  vector<double> weights_ = {1.0};
  NearestNeighborModelRankingScoreIterator ranking_score_iterator_;

  //other
  void recompute(int user);
  void recompute_prediction(int user);
  friend class NearestNeighborModelUpdater;
  FRIEND_TEST(TestNearestNeighborModel, weights);
  FRIEND_TEST(TestNearestNeighborModel, direction);
  
};

#endif /* NEAREST_NEIGHBOR_MODEL_RANKING_SCORE_ITERATOR_H */
