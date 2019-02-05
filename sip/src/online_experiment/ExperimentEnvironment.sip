
class ExperimentEnvironment{
  %TypeHeaderCode
  #include "online_experiment/ExperimentEnvironment.h"
  %End
  public:
    void set_parameters(OnlineExperimentParameters* params);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);

    int get_top_k() const;
  %Docstring
      Returns the top list length in the current experiment. Note that not all
      classes consider this value.
  %End
    int get_min_time() const;
  %Docstring
      Returns the beginning timestamp of evaluation. Elements in the time series
      before that timestamp will not be evaluated. Note that not all evaluator
      classes consider this value.
  %End
    int get_max_time() const;
  %Docstring
      Returns the last timestamp of evaluation. Elements in the time series
      after that timestamp will not be evaluated. Note that not all evaluator
      classes consider this value.

  %End
    bool do_exclude_known() const;
  %Docstring
       Returns whether each user-item pair should be evaluated only at the first
       occurrence, i.e., known user-item pairs should not be evaluated at
       repeated occurrences.
  %End

    RecommenderDataIterator* get_recommender_data_iterator() const;
  %Docstring
      Returns the data iterator containing the time series of the experiment.
  %End
    Random* get_random();
  %Docstring
      Returns the common random generator.
  %End

    bool is_item_new_for_user() const;
  %Docstring
      Returns whether the current item is new for the current user, i.e., this is the
      first occurrence of this user-item pair in the time series. Note that the
      value is updated only when the loggers had been called already.
  %End
    std::vector<int>* get_items();
  %Docstring
      Returns the list of known items.
      Note that the list is updated only when the loggers had been called already.
  %End
    std::vector<int>* get_users();
  %Docstring
      Returns the list of known users.
      Note that the list is updated only when the loggers had been called already.
  %End
    SpMatrix* get_train_matrix();
  %Docstring
      Returns the current train data in a sparse matrix form.
      Note that the matrix is updated only when the loggers had been called already.
  %End
    PopContainer* get_popularity_container();
  %Docstring
      Returns a container containing the popularity statistics of known items.
      Note that the container is updated only when the loggers had been called already.
  %End
    TopPopContainer* get_popularity_sorted_container();
  %Docstring
      Returns a container containing the popularity statistics of known items.
      The items can be acquired in popularity order.
      Note that the container is updated only when the loggers had been called already.
  %End

    void update(RecDat* rec_dat);
  %Docstring
      update(RecDat* rec_dat)
      Updates the container.
  %End

};

