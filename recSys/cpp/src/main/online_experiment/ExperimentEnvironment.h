#ifndef EXPERIMENT_ENVIRONMENT
#define EXPERIMENT_ENVIRONMENT

#include <vector>
#include "../utils/SpMatrix.h"
#include "../recommender_data/RecommenderDataIterator.h"
#include "../recommender_data/macros.h"
#include "../utils/PopContainers.h"
#include "../utils/Random.h"

struct OnlineExperimentParameters{
  int top_k;
  int min_time;
  int max_time;
  bool lookback;
  bool initialize_all;
  int max_user;
  int max_item;
  int random_seed;
};

class ExperimentEnvironment{
  public:
    void set_parameters(OnlineExperimentParameters* params){
      top_k_ = params->top_k;
      min_time_ = params->min_time;
      max_time_ = params->max_time;
      lookback_ = params->lookback;
      initialize_all_ = params->initialize_all;
      random_.set(params->random_seed);
    }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
      recommender_data_iterator_=recommender_data_iterator;
    }

    //global parameters
    int get_top_k() const { return top_k_; }
    int get_min_time() const { return min_time_; }
    int get_max_time() const { return max_time_; }
    bool is_lookback() const { return lookback_; }

    //common objects
    RecommenderDataIterator* get_recommender_data_iterator() const { return recommender_data_iterator_; } //TODO const
    Random* get_random() { return &random_; }

    //common statistics
    bool is_item_new_for_user() const { return item_new_for_user_; }
    vector<int>* get_items() { return &items_; } //TODO const
    vector<int>* get_users() { return &users_; } //TODO const
    SpMatrix* get_train_matrix() { return &train_matrix_; } //TODO const
    PopContainer* get_popularity_container() { return &popularity_container_; } //TODO const
    TopPopContainer* get_popularity_sorted_container() { return &popularity_sorted_container_; } //TODO const

    //update
    void update(RecDat* rec_dat);

  private:
    int top_k_;
    int min_time_;
    int max_time_;
    bool lookback_;
    bool initialize_all_;

    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    Random random_;

    //updatable components
    bool item_new_for_user_;
    vector<int> item_map_;
    vector<int> items_;
    vector<int> user_map_;
    vector<int> users_;
    SpMatrix train_matrix_;
    PopContainer popularity_container_;
    TopPopContainer popularity_sorted_container_;
};


#endif
