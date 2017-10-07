#ifndef ONLINE_EXPERIMENT
#define ONLINE_EXPERIMENT

#include "../recommender_data/RecommenderDataIterator.h"
#include "../general_interfaces/Updater.h"
#include "../loggers/Logger.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "ExperimentEnvironment.h"

class ExperimentEnvironment;
class NeedsExperimentEnvironment;

struct OnlineExperimentParameters{
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
  public:
    OnlineExperiment(OnlineExperimentParameters* parameters);
    ~OnlineExperiment();
    void add_logger(Logger* logger){loggers_.push_back(logger);}
    void add_end_logger(Logger* logger){end_loggers_.push_back(logger);}
    void add_updater(Updater* updater){ updaters_.push_back(updater); }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test(){
      bool ok = true;
      if(recommender_data_iterator_ == NULL){
        ok = false;
        cerr << "OnlineExperiment::recommender_data_iterator_ is not set." << endl;
      }
      if(loggers_.size() == 0){
        //ok = false; //That's not a fatal error.
        cerr << "WARNING: OnlineExperiment::loggers_ is empty." << endl;
      }
      return ok;
    }
    void run();
    void inject_experiment_environment_into(NeedsExperimentEnvironment *object);
  private:
    ExperimentEnvironment* experiment_environment_;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    vector<Updater*> updaters_;
    vector<Logger*> loggers_;
    vector<Logger*> end_loggers_;
};

#endif
