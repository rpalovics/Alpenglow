#ifndef RANK_COMPUTER_H
#define RANK_COMPUTER_H


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

struct RankComputerParameters {
  int random_seed = -1;
  //from exp_env
  int top_k = -1;
  int exclude_known = -1;
};

class RankComputer : public NeedsExperimentEnvironment, public Initializable {
  public:
    RankComputer(){}
    RankComputer(RankComputerParameters* parameters){
      set_parameters(parameters);
    }
    void set_parameters(RankComputerParameters* parameters){
      top_k_=parameters->top_k;
      exclude_known_=parameters->exclude_known;
      random_.set(parameters->random_seed);
    }
    virtual ~RankComputer(){}
    void set_model(Model* model){ model_ = model;} 
    void set_model_filter(ModelFilter* model_filter){ model_filter_ = model_filter; }
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_ = train_matrix; }
    void set_top_pop_container(TopPopContainer* popularity_sorted_container){ popularity_sorted_container_ = popularity_sorted_container; }
    void set_items(const vector<int> *items){ items_ = items; }
    bool self_test(){
      bool ok=random_.self_test();
      if(experiment_environment_==NULL){cerr << "RankComputer:experiment_environment is NULL." << endl; }//thats just a warning
      if(top_k_<0){ok=false;cerr<<"RankComputer::top_k is not set properly." << endl;}
      if(model_==NULL){ok=false;cerr<<"RankComputer::model is not set." << endl;}
      if(popularity_sorted_container_==NULL and model_filter_==NULL){ok=false;cerr<<"RankComputer::popularity_sorted_container_ is not set, RankComputer::model_filter is not set." << endl;}
      if(train_matrix_==NULL){ok=false;cerr<<"RankComputer::train_matrix_ is not set." << endl;}
      return ok;
    }
    int get_rank(RecDat*);
  protected:
    bool autocalled_initialize() override {
      if(top_k_==-1){
        if(experiment_environment_==NULL) return false;
        top_k_=experiment_environment_->get_top_k();
      }
      if(train_matrix_==NULL){
        if(exclude_known_==-1){
          if(experiment_environment_==NULL) return false;
          exclude_known_=experiment_environment_->get_exclude_known();
        }
        if(exclude_known_){
          if(experiment_environment_==NULL) return false;
          train_matrix_=experiment_environment_->get_train_matrix();
        } else {
          train_matrix_=&empty_matrix_;
        }
      }
      if(popularity_sorted_container_==NULL){
        if(experiment_environment_==NULL) return false;
        popularity_sorted_container_=experiment_environment_->get_popularity_sorted_container();
      }

      RankingScoreIteratorProvider* ranking_model = dynamic_cast<RankingScoreIteratorProvider*>(model_);
      if(ranking_model){
        ranking_model_ = ranking_model;
      }
      TopListRecommender* toplist_model = dynamic_cast<TopListRecommender*>(model_);
      if(toplist_model){
        toplist_model_ = toplist_model;
      }
      ToplistFromRankingScoreRecommender* tfrsr_model = dynamic_cast<ToplistFromRankingScoreRecommender*>(model_);
      if(tfrsr_model){
        tfrsr_model_ = tfrsr_model;
      }

      if(items_==NULL && ranking_model!=NULL){
        if(experiment_environment_==NULL) return false;
        items_ = experiment_environment_->get_items();
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
    const vector<int>* items_ = NULL;
    vector<pair<int,double>>* itemlist_ = NULL;
    Model* model_ = NULL;
    RankingScoreIteratorProvider* ranking_model_ = NULL;
    TopListRecommender* toplist_model_ = NULL;
    ToplistFromRankingScoreRecommender* tfrsr_model_ = NULL; //not NULL->"fake" toplist model
    const SpMatrix* train_matrix_ = NULL;
    SpMatrix empty_matrix_;
    const TopPopContainer* popularity_sorted_container_ = NULL;
    ModelFilter* model_filter_ = NULL;
    Random random_;
    int top_k_ = -1;
    int exclude_known_ = -1;
};

#endif /* RANK_COMPUTER_H */
