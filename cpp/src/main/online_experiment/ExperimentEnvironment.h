#ifndef EXPERIMENT_ENVIRONMENT
#define EXPERIMENT_ENVIRONMENT

#include <vector>
#include "../utils/SpMatrix.h"
#include "../recommender_data/RecommenderDataIterator.h"
#include "../recommender_data/macros.h"
#include "../utils/PopContainers.h"
#include "../utils/Random.h"
#include "OnlineExperiment.h"

//SIP_AUTOCONVERT

struct OnlineExperimentParameters;

class ExperimentEnvironment{
  public:
    void set_parameters(OnlineExperimentParameters* params);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
      recommender_data_iterator_=recommender_data_iterator;
    }

    //global parameters
    int get_top_k() const { return top_k_; }
    /**
      Returns the top list length in the current experiment. Note that not all
      classes consider this value.
    */
    int get_min_time() const { return min_time_; } //TODO is this parameter in use?
    /**
      Returns the beginning timestamp of evaluation. Elements in the time series
      before that timestamp will not be evaluated. Note that not all evaluator
      classes consider this value.
    */
    int get_max_time() const { return max_time_; } //TODO is this parameter in use?
    /**
      Returns the last timestamp of evaluation. Elements in the time series
      after that timestamp will not be evaluated. Note that not all evaluator
      classes consider this value.
      //The experiment will be terminated. //TODO should be, but it isn't.
    */
    bool do_exclude_known() const { return exclude_known_; }
    /**
       Returns whether each user-item pair should be evaluated only at the first
       occurrence, i.e., known user-item pairs should not be evaluated at
       repeated occurrences.
    */

    //common objects
    RecommenderDataIterator* get_recommender_data_iterator() const { return recommender_data_iterator_; } //TODO const
    /**
      Returns the data iterator containing the time series of the experiment.
    */
    Random* get_random() { return &random_; }
    /**
      Returns the common random generator.
    */

    //common statistics
    bool is_item_new_for_user() const { return item_new_for_user_; }
    /**
      Returns whether the current item is new for the current user, i.e., this is the
      first occurrence of this user-item pair in the time series. Note that the
      value is updated only when the loggers had been called already.
    */
    vector<int>* get_items() { return &items_; } //TODO return const value, classes should not change the vector
    /**
      Returns the list of known items.
      Note that the list is updated only when the loggers had been called already.
    */
    vector<int>* get_users() { return &users_; } //TODO return const value, classes should not change the vector
    /**
      Returns the list of known users.
      Note that the list is updated only when the loggers had been called already.
    */
    SpMatrix* get_train_matrix() { return &train_matrix_; } //TODO const
    /**
      Returns the current train data in a sparse matrix form.
      Note that the matrix is updated only when the loggers had been called already.
    */
    PopContainer* get_popularity_container() { return &popularity_container_; } //TODO const
    /**
      Returns a container containing the popularity statistics of known items.
      Note that the container is updated only when the loggers had been called already.
    */
    TopPopContainer* get_popularity_sorted_container() { return &popularity_sorted_container_; } //TODO const
    /**
      Returns a container containing the popularity statistics of known items.
      The items can be acquired in popularity order.
      Note that the container is updated only when the loggers had been called already.
    */

    //update
    void update(RecDat* rec_dat);
    /**
      update(RecDat* rec_dat)
      Updates the container.
    */

  private:
    int top_k_;
    int min_time_;
    int max_time_;
    bool exclude_known_;
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
