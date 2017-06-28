struct OnlineExperimentParameters{
  %TypeHeaderCode
  #include "online_experiment/OnlineExperiment.h"
  %End
  int seed;
};
class OnlineExperiment{
  %TypeHeaderCode
  #include "online_experiment/OnlineExperiment.h"
  %End
  public:
    OnlineExperiment(OnlineExperimentParameters * parameters);
    ~OnlineExperiment();
    void add_logger(Logger* logger);
    void set_online_data_updater(OnlineDataUpdater* updater);
    void set_online_recommender(OnlineRecommender* recommender);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test();
    void run() throw (PythonKeyboardInterruptException) /ReleaseGIL/;
    void inject_experiment_environment_into(INeedExperimentEnvironment *object);
};
