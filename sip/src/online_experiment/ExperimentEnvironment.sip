
class ExperimentEnvironment{
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
    int get_min_time() const;
  %Docstring
      Returns
      -------
      int
          The beginning timestamp of evaluation. Elements in the time series before that timestamp will not be evaluated. Note that not all evaluator classes consider this value.
  %End
    int get_max_time() const;
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
          The maximal user id int the whole experiment.  Use together with initialize_all=true.
  %End
    int get_max_item_id() const;
  %Docstring
       Returns
       -------
       int
          The maximal item id int the whole experiment.  Use together with initialize_all=true.
  %End

    RecommenderDataIterator* get_recommender_data_iterator() const;
  %Docstring
      Returns
      -------
      RecommenderDataIterator*
          A pointer to the data iterator containing the time series of the experiment. 
  %End
    Random* get_random();
  %Docstring
      Returns
      -------
      Random*
          A pointer to the common random generator.
  %End

    bool is_item_new_for_user() const;
  %Docstring
      Returns
      -------
      bool
          Whether the current item is new for the current user, i.e., this is the first occurrence of this user-item pair in the time series. Note that the value is updated only when the loggers had been called already.
  %End
    std::vector<int>* get_items();
  %Docstring
      Returns
      -------
      std::vector<int>*
          A pointer to the list of known items. In the online experiment, the list is updated by this class for each sample after the call of the loggers and before the call to the updaters.
  %End
    std::vector<int>* get_users();
  %Docstring
      Returns
      -------
      std::vector<int>*
          A pointer to the list of known users.
  %End
    SpMatrix* get_train_matrix();
  %Docstring
      Returns
      -------
      SpMatrix*
          A pointer to the current training data in a sparse matrix form.
  %End
    PopContainer* get_popularity_container();
  %Docstring
      Returns
      -------
      PopContainer*
          A pointer to a container containing the popularity statistics of known items.
  %End
    TopPopContainer* get_popularity_sorted_container();
  %Docstring
      Returns
      -------
      TopPopContainer*
          A pointer to a container containing the popularity statistics of known items.  The items can be acquired in popularity order.
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

};

