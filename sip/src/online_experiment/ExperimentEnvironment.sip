
class ExperimentEnvironment /NoDefaultCtors/{
  %TypeHeaderCode
  #include "online_experiment/ExperimentEnvironment.h"
  %End
  public:
  %Docstring
      Class that stores, updates and serves common simulation data and
      parameters, e.g. length of the top list, dataset and popularity of the
      items.

      In the online experiment, the central class
      :py:class:`alpenglow.cpp.OnlineExperiment` updates this class and the
      common statistic containers. This class is updated after calling loggers
      and before calling updaters (for each sample). See details there. Other
      objects are not allowed to modify this class or the statistic containers,
      even if they have non-const access (exception: the common Random).
  %End
    void set_parameters(OnlineExperimentParameters* params);
  %Docstring
        Sets the parameters of the experiment. Called by :py:class:`alpenglow.cpp.OnlineExperiment`.
  %End
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
  %Docstring
        Sets the dataset (the time series) of the experiment. Called by :py:class:`alpenglow.cpp.OnlineExperiment`.
  %End

    int get_top_k() const;
  %Docstring
      Returns
      -------
      int
          The top list length in the current experiment. Note that not all classes consider this value.
  %End
    int get_evaluation_start_time() const;
  %Docstring
      Returns
      -------
      int
          The beginning timestamp of evaluation. Elements in the time series before that timestamp will not be evaluated. Note that not all evaluator classes consider this value.
  %End
    int get_experiment_termination_time() const;
  %Docstring
      Returns
      -------
      int
          The last timestamp of evaluation. Elements in the time series after that timestamp will not be evaluated. Note that not all evaluator classes consider this value.  
  %End
    bool get_exclude_known() const;
  %Docstring
       Returns
       -------
       bool
           Whether each user-item pair should be evaluated only at the first occurrence, i.e., known user-item pairs should not be evaluated at repeated occurrences.
  %End
    bool get_initialize_all() const;
  %Docstring
       Returns
       -------
       bool
           Whether all the users and items exist from the beginning of the experiment, or they appear only when they are mentioned first in a sample. If set, recode the dataset so that the users and items are numbered starting 0 or 1 continuously. Skipped ids are treated as existing too.
  %End
    int get_max_user_id() const;
  %Docstring
       Returns
       -------
       int
          The maximal user id int the whole experiment.
  %End
    int get_max_item_id() const;
  %Docstring
       Returns
       -------
       int
          The maximal item id int the whole experiment.
  %End

    const RecommenderDataIterator* get_recommender_data_iterator() const;
  %Docstring
      Returns
      -------
      RecommenderDataIterator*
          A pointer to the data iterator containing the time series of the experiment. 
  %End

    bool is_item_new_for_user() const;
  %Docstring
      Returns
      -------
      bool
          Whether the current item is new for the current user, i.e., this is the first occurrence of this user-item pair in the time series. Note that the value is updated only when the loggers had been called already.
  %End
    bool is_item_existent(int item) const;
  %Docstring
      Returns
      -------
      bool
          Whether the item exists.  If ``initialize_all==True``, returns constant true value for items<=max_item_id, because all items exsist from the begininning of the experiment.  Note that new items come into existence after te call to loggers, before the call to updaters.
  %End
    bool is_new_item() const;
  %Docstring
      Returns
      -------
      bool
          Whether the current item is new, i.e. come to existence with the current sample.  If ``initialize_all==True``, returns constant false value, because all items exsist from the begininning of the experiment.  Note that new items come into existence after te call to loggers, before the call to updaters.
  %End
    bool is_first_occurrence_of_item() const;
  %Docstring
      Returns
      -------
      bool
          Whether the current item is mentioned for the first time in the current sample.  If  ``initialize_all==False``, equals to :py:meth:`is_new_item()`.
  %End
    bool is_user_existing(int user) const;
  %Docstring
      Returns
      -------
      bool
          Whether the user exists.  If ``initialize_all==True``, returns constant true value for users<=max_user_id, because all users exsist from the begininning of the experiment.  Note that new users come into existence after te call to loggers, before the call to updaters.
  %End
    bool is_new_user() const;
  %Docstring
      Returns
      -------
      bool
          Whether the current user is new, i.e. come to existence with the current sample.  If ``initialize_all==True``, returns constant false value, because all users exsist from the begininning of the experiment.  Note that new users come into existence after te call to loggers, before the call to updaters.
  %End
    bool is_first_occurrence_of_user() const;
  %Docstring
      Returns
      -------
      bool
          Whether the current user is mentioned for the first time in the current sample.  If  ``initialize_all==False``, equals to :py:meth:`is_new_user()`.
  %End
    const std::vector<int>* get_items() const;
  %Docstring
      Returns
      -------
      std::vector<int>*
          A pointer to the list of known items. In the online experiment, the list is updated by this class for each sample after the call of the loggers and before the call to the updaters.  If ``initialize_all==True``, the list is filled with items at the beginning of the experiment.
  %End
    const std::vector<int>* get_users() const;
  %Docstring
      Returns
      -------
      std::vector<int>*
          A pointer to the list of known users.  In the online experiment, the list is updated by this class for each sample after the call of the loggers and before the call to the updaters.  If ``initialize_all==True``, the list is filled with users at the beginning of the experiment.
  %End
    const SpMatrix* get_train_matrix() const;
  %Docstring
      Returns
      -------
      SpMatrix*
          A pointer to the current training data in a sparse matrix form.
  %End
    const PopContainer* get_popularity_container() const;
  %Docstring
      Returns
      -------
      PopContainer*
          A pointer to a container containing the popularity statistics of known items.
  %End
    const TopPopContainer* get_popularity_sorted_container() const;
  %Docstring
      Returns
      -------
      TopPopContainer*
          A pointer to a container containing the popularity statistics of known items.  The items can be acquired in popularity order.  The container contains all known items.
  %End

    void update(RecDat* rec_dat);
  %Docstring
      update(RecDat* rec_dat)
      Updates the container.

      In the online experiment, :py:class:`alpenglow.cpp.OnlineExperiment` calls this function after the call to the loggers and before the call to the updaters. Other classes should not call this function.
      Parameters
      ----------
      rec_dat : RecDat*
         The newest available sample of the experiment.

  %End
    bool self_test();

};

