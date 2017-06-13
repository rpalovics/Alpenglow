#ifndef EXPERIMENT_ENVIRONMENT
#define EXPERIMENT_ENVIRONMENT

#include <vector>
#include "../utils/SpMatrix.h"
#include "../recommender_data/RecommenderDataIterator.h"
#include "../utils/PopContainers.h"
#include "../utils/Random.h"
struct ExperimentEnvironmentParameters{
  int top_k;
  int min_time;
  int max_time;
  bool lookback;
  bool initialize_all;
  int random_seed;
  RecommenderDataIterator* recommender_data_iterator;
};

class ExperimentEnvironment{
  public:
    ExperimentEnvironment(){} //TODO shoud be parameterized
    void set_parameters(ExperimentEnvironmentParameters* params){
      top_k_ = params->top_k;
      min_time_ = params->min_time;
      max_time_ = params->max_time;
      lookback_ = params->lookback;
      initialize_all_ = params->initialize_all;
      recommender_data_iterator_ = params->recommender_data_iterator;
      random_.set(params->random_seed);
    }

    //global parameters
    int get_top_k(){ return top_k_; } //TODO const
    int get_min_time(){ return min_time_; } //TODO const
    int get_max_time(){ return max_time_; } //TODO const
    bool is_lookback(){ return lookback_; } //TODO const

    //common objects
    RecommenderDataIterator* get_recommender_data_iterator(){ return recommender_data_iterator_; } //TODO const
    Random* get_random(){return &random_; }

    //common statistics
    bool is_item_new_for_user(){ return true; } //TODO METHOD STUB
    vector<int>* get_items(){ return &items_; } //TODO const
    vector<int>* get_users(){ return &users_; } //TODO const
    SpMatrix* get_train_matrix(){ return &train_matrix_; } //TODO const
    PopContainer* get_popularity_container(){ return &popularity_conatiner_; } //TODO const
    TopPopContainer* get_popularity_sorted_container(){ return &popularity_sorted_conatiner_; } //TODO const

    //update
    void update(RecDat* rec_dat);

  private:
    int top_k_;
    int min_time_;
    int max_time_;
    bool lookback_;
    bool initialize_all_;

    RecommenderDataIterator* recommender_data_iterator_;

    bool item_new_for_user;
    vector<int> items_;
    vector<int> users_;
    SpMatrix train_matrix_;
    PopContainer popularity_conatiner_;
    TopPopContainer popularity_sorted_conatiner_;
    Random random_;
};


#endif
