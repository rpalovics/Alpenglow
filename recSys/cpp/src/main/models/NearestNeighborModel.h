#ifndef NEAREST_NEIGHBOR_MODEL
#define NEAREST_NEIGHBOR_MODEL

#include <iostream>
#include <gtest/gtest_prod.h>
#include "Model.h"
#include "../utils/UserHistory.h"
#include "../filters/ModelFilter.h"
#include "../recommender_data/macros.h"

struct NearestNeighborModelParameters {
  double gamma;
  string norm;
  string direction;
  double gamma_threshold;
  int num_of_neighbors;

  NearestNeighborModelParameters(){
    gamma=-1;
    norm="";
    direction="";
    gamma_threshold=-1;
    num_of_neighbors=-1;
  }
};
class NearestNeighborModel : public Model , public ModelFilter {
  public:
    NearestNeighborModel(NearestNeighborModelParameters* params):
      gamma_(params->gamma),
      gamma_threshold_(params->gamma_threshold),
      num_of_neighbors_(params->num_of_neighbors),
      norm_type_(params->direction==""?"num":params->norm),
      direction_(params->direction==""?"forward":params->direction)
    {
      active_user_=-1;
      weights_.push_back(1.0);
    }
    void add(RecDat* rec_dat) override {}
    double prediction(RecDat* rec_dat) override;
    vector<pair<int,double>>* get_personalized_items(int user) override;
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
    vector<pair<ITEM,SCORE>> active_user_filter_;
    vector<SCORE> active_user_predictions_;
    int active_user_;
    vector<double> weights_;

    //other
    void recompute_filter(int user);
    friend class NearestNeighborModelUpdater;
    FRIEND_TEST(TestNearestNeighborModel, weights);
    FRIEND_TEST(TestNearestNeighborModel, direction);
};

#endif
