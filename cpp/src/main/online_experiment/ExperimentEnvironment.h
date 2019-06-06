#ifndef EXPERIMENT_ENVIRONMENT_H
#define EXPERIMENT_ENVIRONMENT_H

#include <vector>
#include <memory>
#include "../utils/SpMatrix.h"
#include "../recommender_data/RecommenderDataIterator.h"
#include "../recommender_data/macros.h"
#include "../utils/PopContainers.h"
#include "../utils/Random.h"
#include "OnlineExperiment.h"

//SIP_AUTOCONVERT

struct OnlineExperimentParameters;

class ExperimentEnvironment{ //SIP_NODEFAULTCTORS
  public:
    /**
      Class that stores, updates and serves common simulation data and
      parameters, e.g. length of the top list, dataset and popularity of the
      items.

      In the online experiment, the central class
      :py:class:`alpenglow.cpp.OnlineExperiment` updates this class and the
      common statistic containers. This class is updated after calling loggers
      and before calling updaters (for each sample). See details there. Other
      objects are not allowed to modify this class or the statistic containers,
      even if they have non-const access (exception: the common Random).
    */
    void set_parameters(OnlineExperimentParameters* params);
    /**
        Sets the parameters of the experiment. Called by :py:class:`alpenglow.cpp.OnlineExperiment`.
    */
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
      recommender_data_iterator_=recommender_data_iterator;
    }
    /**
        Sets the dataset (the time series) of the experiment. Called by :py:class:`alpenglow.cpp.OnlineExperiment`.
    */

    //global parameters
    int get_top_k() const { return top_k_; }
    /**
      Returns
      -------
      int
          The top list length in the current experiment. Note that not all classes consider this value.
    */
    int get_evaluation_start_time() const { return evaluation_start_time_; }
    /**
      Returns
      -------
      int
          The beginning timestamp of evaluation. Elements in the time series before that timestamp will not be evaluated. Note that not all evaluator classes consider this value.
    */
    int get_experiment_termination_time() const { return experiment_termination_time_; }
    /**
      Returns
      -------
      int
          The last timestamp of evaluation. Elements in the time series after that timestamp will not be evaluated. Note that not all evaluator classes consider this value.  //The experiment will be terminated. //TODO should be, but it isn't.
    */
    bool get_exclude_known() const { return exclude_known_; }
    /**
       Returns
       -------
       bool
           Whether each user-item pair should be evaluated only at the first occurrence, i.e., known user-item pairs should not be evaluated at repeated occurrences.
    */
    bool get_initialize_all() const { return initialize_all_; }
    /**
       Returns
       -------
       bool
           Whether all the users and items exist from the beginning of the experiment, or they appear only when they are mentioned first in a sample. If set, recode the dataset so that the users and items are numbered starting 0 or 1 continuously. Skipped ids are treated as existing too.
    */
    int get_max_user_id() const { return max_user_; }
    /**
       Returns
       -------
       int
          The maximal user id int the whole experiment.
    */
    int get_max_item_id() const { return max_item_; }
    /**
       Returns
       -------
       int
          The maximal item id int the whole experiment.
    */

    //common objects
    const RecommenderDataIterator* get_recommender_data_iterator() const { return recommender_data_iterator_; } //TODO const
    /**
      Returns
      -------
      RecommenderDataIterator*
          A pointer to the data iterator containing the time series of the experiment. 
    */

    //common statistics
    bool is_item_new_for_user() const { return item_new_for_user_; }
    /**
      Returns
      -------
      bool
          Whether the current item is new for the current user, i.e., this is the first occurrence of this user-item pair in the time series. Note that the value is updated only when the loggers had been called already.
    */
    bool is_item_existent(int item) const { return (initialize_all_ && item<=max_item_) || GET_VECTORMAP(item_map_,item,false); }
    /**
      Returns
      -------
      bool
          Whether the item exists.  If ``initialize_all==True``, returns constant true value for items<=max_item_id, because all items exsist from the begininning of the experiment.  Note that new items come into existence after te call to loggers, before the call to updaters.
    */
    bool is_new_item() const { return !initialize_all_ && first_occurrence_of_item_; }
    /**
      Returns
      -------
      bool
          Whether the current item is new, i.e. come to existence with the current sample.  If ``initialize_all==True``, returns constant false value, because all items exsist from the begininning of the experiment.  Note that new items come into existence after te call to loggers, before the call to updaters.
    */
    bool is_first_occurrence_of_item() const { return first_occurrence_of_item_; }
    /**
      Returns
      -------
      bool
          Whether the current item is mentioned for the first time in the current sample.  If  ``initialize_all==False``, equals to :py:meth:`is_new_item()`.
    */
    bool is_user_existing(int user) const { return (initialize_all_ && user<=max_user_) || GET_VECTORMAP(user_map_,user,false); }
    /**
      Returns
      -------
      bool
          Whether the user exists.  If ``initialize_all==True``, returns constant true value for users<=max_user_id, because all users exsist from the begininning of the experiment.  Note that new users come into existence after te call to loggers, before the call to updaters.
    */
    bool is_new_user() const { return !initialize_all_ && first_occurrence_of_user_; }
    /**
      Returns
      -------
      bool
          Whether the current user is new, i.e. come to existence with the current sample.  If ``initialize_all==True``, returns constant false value, because all users exsist from the begininning of the experiment.  Note that new users come into existence after te call to loggers, before the call to updaters.
    */
    bool is_first_occurrence_of_user() const { return first_occurrence_of_user_; }
    /**
      Returns
      -------
      bool
          Whether the current user is mentioned for the first time in the current sample.  If  ``initialize_all==False``, equals to :py:meth:`is_new_user()`.
    */
    const vector<int>* get_items() const { return &items_; }
    /**
      Returns
      -------
      vector<int>*
          A pointer to the list of known items. In the online experiment, the list is updated by this class for each sample after the call of the loggers and before the call to the updaters.  If ``initialize_all==True``, the list is filled with items at the beginning of the experiment.
    */
    const vector<int>* get_users() const { return &users_; }
    /**
      Returns
      -------
      vector<int>*
          A pointer to the list of known users.  In the online experiment, the list is updated by this class for each sample after the call of the loggers and before the call to the updaters.  If ``initialize_all==True``, the list is filled with users at the beginning of the experiment.
    */
    const SpMatrix* get_train_matrix() const { return &train_matrix_; }
    /**
      Returns
      -------
      SpMatrix*
          A pointer to the current training data in a sparse matrix form.
    */
    const PopContainer* get_popularity_container() const { return &popularity_container_; }
    /**
      Returns
      -------
      PopContainer*
          A pointer to a container containing the popularity statistics of known items.
    */
    const TopPopContainer* get_popularity_sorted_container() const { return &popularity_sorted_container_; }
    /**
      Returns
      -------
      TopPopContainer*
          A pointer to a container containing the popularity statistics of known items.  The items can be acquired in popularity order.  The container contains all known items.
    */

    //update
    void update(RecDat* rec_dat);
    /**
      update(RecDat* rec_dat)
      Updates the container.
      
      In the online experiment, :py:class:`alpenglow.cpp.OnlineExperiment` calls this function after the call to the loggers and before the call to the updaters. Other classes should not call this function.
      Parameters
      ----------
      rec_dat : RecDat*
         The newest available sample of the experiment.

    */
    bool self_test(){
      bool ok = true;
      if (top_k_<0) ok=false;
      if (evaluation_start_time_!=0 && experiment_termination_time_!=0
          && evaluation_start_time_>=experiment_termination_time_) ok=false;
      if (initialize_all_ && (max_user_<0 || max_item_<0)) ok=false;
      if (recommender_data_iterator_==NULL) ok=false;
      return ok;
    }

  private:
    int top_k_ = -1;
    int evaluation_start_time_ = -1;
    int experiment_termination_time_ = -1;
    bool exclude_known_ = true;
    bool initialize_all_ = false;
    int max_user_ = -1;
    int max_item_ = -1;

    RecommenderDataIterator* recommender_data_iterator_ = NULL;

    //updatable components
    bool item_new_for_user_ = true;
    bool first_occurrence_of_item_ = true;
    bool first_occurrence_of_user_ = true;
    vector<int> item_map_;
    vector<int> items_;
    vector<int> user_map_;
    vector<int> users_;
    SpMatrix train_matrix_;
    PopContainer popularity_container_;
    TopPopContainer popularity_sorted_container_;
};


#endif /* EXPERIMENT_ENVIRONMENT_H */
