#ifndef RANKING
#define RANKING


#include "../utils/SpMatrix.h"
#include "../utils/PopContainers.h"
#include "../filters/ModelFilter.h"
#include "../models/Model.h"
#include "../recommender_data/RecommenderData.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../models/RankingScoreIterator.h"
#include "../models/TopListRecommender.h"
#include <numeric> 
#include "../utils/Random.h"

struct RankComputerParameters{
  int top_k;
  int random_seed;
  RankComputerParameters(){
    top_k = -1;
    random_seed = -1;
  }
};

class RankComputer : public NeedsExperimentEnvironment, public Initializable{
  public:
    RankComputer(RankComputerParameters* parameters){
      top_k_=(parameters->top_k==-1?parameters->top_k:parameters->top_k);
      random_.set(parameters->random_seed);
    }
    void set_model(Model* model){ model_ = model;} 
    void set_model_filter(ModelFilter* model_filter){ model_filter_ = model_filter; }
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_ = train_matrix; }
    void set_top_pop_container(TopPopContainer* popularity_sorted_container){ popularity_sorted_container_ = popularity_sorted_container; }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override {
      experiment_environment_=experiment_environment;
    }
    void set_items(vector<int> *items){
      items_ = items;
    }
    bool self_test(){
      bool ok=true;
      if (experiment_environment_==NULL){cerr << "RankComputer:experiment_environment is NULL." << endl; }
      if(top_k_<0){ok=false;cerr<<"RankComputer::top_k is not set properly." << endl;}
      if(model_==NULL){ok=false;cerr<<"RankComputer::model is not set." << endl;}
      if(popularity_sorted_container_==NULL and model_filter_==NULL){ok=false;cerr<<"RankComputer::popularity_sorted_container_ is not set, RankComputer::model_filter is not set." << endl;}
      if(train_matrix_==NULL){ok=false;cerr<<"RankComputer::train_matrix_ is not set." << endl;}
      return ok;
    }
    int get_rank(RecDat*);
  protected:
    bool autocalled_initialize() override {
      if(train_matrix_==NULL) train_matrix_=experiment_environment_->get_train_matrix();
      if(popularity_sorted_container_==NULL){
        popularity_sorted_container_=experiment_environment_->get_popularity_sorted_container();
      }
      if(items_==NULL) items_ = experiment_environment_->get_items();

      RankingScoreIteratorProvider* ranking_model = dynamic_cast<RankingScoreIteratorProvider*>(model_);
      if(ranking_model){
        ranking_model_ = ranking_model;
      }

      TopListRecommender* toplist_model = dynamic_cast<TopListRecommender*>(model_);
      if(toplist_model){
        toplist_model_ = toplist_model;
      }

      
      return true;
    }
    int get_rank_bruteforce(RecDat*);
    int get_rank_ranking_model(RecDat*);
    int get_rank_toplist_model(RecDat*);
    void itemlist_init(RecDat* rec_dat);
    bool itemlist_next(RecDat* rec_dat);
    int itemlist_index_;
    int itemlist_max_index_;
    int itemlist_positive_item_;
    vector<int> *items_ = NULL;
    vector<pair<int,double>>* itemlist_ = NULL;
    Model* model_ = NULL;
    RankingScoreIteratorProvider* ranking_model_ = NULL;
    TopListRecommender* toplist_model_ = NULL;
    SpMatrix* train_matrix_ = NULL;
    TopPopContainer* popularity_sorted_container_ = NULL;
    ModelFilter* model_filter_ = NULL;
    ExperimentEnvironment* experiment_environment_ = NULL;
    Random random_;
    int top_k_;
};

#endif
