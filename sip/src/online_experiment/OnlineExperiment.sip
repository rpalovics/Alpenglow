
struct OnlineExperimentParameters{
  %TypeHeaderCode
  #include "online_experiment/OnlineExperiment.h"
  %End
  int top_k;
  int min_time;
  int max_time;
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
  The central class of the experiment.
  Calls the loggers and updaters for each sample, and updates experiment_environment.

  See :py:class:`alpenglow.OnlineExperiment.OnlineExperiment`.
  %End
  public:
    OnlineExperiment(OnlineExperimentParameters* parameters);
    ~OnlineExperiment();
    void add_logger(Logger* logger);
  %Docstring
    Add a logger instance.
    Loggers will be called for each sample before updating the environment and calling updaters.
  %End
    void add_end_logger(Logger* logger);
  %Docstring
    Add a logger instance, that will be called once at the end of the experiment.
  %End
    void add_updater(Updater* updater);
  %Docstring
    Add an updater. Updaters will be called for each sample after updting the environment.
  %End
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
  %Docstring
    Set the dataset of the experiment.
  %End
    bool self_test();
    void run() throw (PythonKeyboardInterruptException) /ReleaseGIL/;
  %Docstring
    Run the experiment.
  %End
    void inject_experiment_environment_into(NeedsExperimentEnvironment *object);
};

