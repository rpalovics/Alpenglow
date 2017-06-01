#ifndef EXPERIMENT_ENVIRONMENT
#define EXPERIMENT_ENVIRONMENT

#include <vector>
#include "../utils/SpMatrix.h"
#include "../recommender_data/RecommenderDataIterator.h"
#include "../utils/PopContainers.h"
struct ExperimentEnvironmentParameters{
  int top_k;
  int min_time;
  int max_time;
  bool eval_first;
  bool initialize_all;
  RecommenderDataIterator* recommender_data_iterator;
};

class ExperimentEnvironment{
  public:
    ExperimentEnvironment(ExperimentEnvironmentParameters* params){
      top_k_ = params->top_k;
      min_time_ = params->min_time;
      max_time_ = params->max_time;
      eval_first_ = params->eval_first;
      initialize_all_ = params->initialize_all;
      recommender_data_iterator_ = params->recommender_data_iterator;
    }

    //global parameters
    int get_top_k(); //TODO const
    int get_min_time(); //TODO const
    int get_max_time(); //TODO const
    bool get_eval_first(); //TODO const

    //dataset
    RecommenderDataIterator* get_recommender_data_iterator(); //TODO const

    //common containers
    vector<int>* get_items(); //TODO const
    vector<int>* get_users(); //TODO const
    SpMatrix* get_train_matrix(); //TODO const
    PopContainer* get_popularity_container(); //TODO const
    TopPopContainer* get_popularity_sorted_container(); //TODO const

  private:
    int top_k_;
    int min_time_;
    int max_time_;
    bool eval_first_;
    bool initialize_all_;

    RecommenderDataIterator* recommender_data_iterator_;

    vector<int> items_;
    vector<int> users_;
    SpMatrix train_matrix_;
    PopContainer popularity_conatiner_;
    TopPopContainer popularity_sorted_conatiner_;
};


#endif
