struct OnlineExperimentParameters{
  %TypeHeaderCode
  #include "online_experiment/ExperimentEnvironment.h"
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
  public:
    OnlineExperiment(OnlineExperimentParameters * parameters);
    ~OnlineExperiment();
    void add_logger(Logger* logger);
    //void set_online_data_updater(OnlineDataUpdater* updater);
    void add_updater(Updater* updater);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test();
    void run() throw (PythonKeyboardInterruptException) /ReleaseGIL/;
    void inject_experiment_environment_into(NeedsExperimentEnvironment *object);
};
