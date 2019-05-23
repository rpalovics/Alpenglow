
struct OnlineExperimentParameters {
  %TypeHeaderCode
  #include "online_experiment/OnlineExperiment.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.OnlineExperiment`. See documentation there.
  %End
  int top_k;
  int evaluation_start_time;
  int experiment_termination_time;
  bool exclude_known;
  bool initialize_all;
  int max_user;
  int max_item;
  int random_seed;
};

class OnlineExperiment{
  %TypeHeaderCode
  #include "online_experiment/OnlineExperiment.h"
  %End
  %Docstring
  The central class of the online experiment.

  It queries samples from the dataset, then one-by-one for each sample

  * calls loggers that are set using :py:meth:`add_logger`,
  * updates the environment and common statistics, see :py:class:`alpenglow.cpp.ExperimentEnvironment`,
  * calls the updaters that are set using :py:meth:`add_updater`.

  At the end of the experiment, it calls end loggers that are set using :py:meth:`add_end_logger`.

  See :py:class:`alpenglow.OnlineExperiment.OnlineExperiment` for details.
  %End
  public:
    OnlineExperiment(OnlineExperimentParameters* parameters);
    ~OnlineExperiment();
    void add_logger(Logger* logger);
  %Docstring
      add_logger(Logger* logger)
      Adds a logger instance.

      Parameters
      ----------
      logger : Logger*
          Pointer to the logger to be added.
  %End
    void add_end_logger(Logger* logger);
  %Docstring
      add_end_logger(Logger* logger)
      Adds a logger instance, that will be called once at the end of the experiment.

      Parameters
      ----------
      logger : Logger*
          Pointer to the logger to be added.
  %End
    void add_updater(Updater* updater);
  %Docstring
      add_updater(Updater* updater)
      Adds an updater.

      Parameters
      ----------
      updater : Updater*
          Pointer to the updater to be added.
  %End
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
  %Docstring
      set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator)
      Sets the dataset of the experiment.

      Parameters
      ----------
      recommender_data_iterator : RecommenderDataIterator*
          Pointer to the dataset.
  %End
    bool self_test();
  %Docstring
      Tests if the dataset is set.

      Furthermore, the test produces a warning message if no loggers are set
      because in that case the the experiment will produce no output. 

      Returns
      -------
      bool
         Whether the tests were successful. 
  %End
    void run() throw (PythonKeyboardInterruptException) /ReleaseGIL/;
  %Docstring
      Runs the experiment.
  %End
    void inject_experiment_environment_into(NeedsExperimentEnvironment* object);
  %Docstring
      inject_experiment_environment_into(NeedsExperimentEnvironment* object)
      Sets the experiment environment into another object that requires it.

      In the online experiment, this method is automatically called with all the
      objects that implement :py:class:`alpenglow.cpp.NeedsExperimentEnvironment`,
      injecting the dependency where it is necessary. See the code of
      :py:class:`alpenglow.OnlineExperiment.OnlineExperiment` for details.
  %End
};

