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
    void add_logger(Logger* logger /Transfer/);
    void set_online_data_updater(OnlineDataUpdater* updater /Transfer/);
    void set_online_recommender(OnlineRecommender* recommender /Transfer/);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator /Transfer/);
    bool selfTest();
    void run() throw (PythonKeyboardInterruptException);
};
