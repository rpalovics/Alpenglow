#ifndef ONLINE_EXPERIMENT
#define ONLINE_EXPERIMENT

#include "../recommender_data/RecommenderDataIterator.h"
#include "../online_recommender/OnlineRecommender.h"
#include "../online_data_updater/OnlineDataUpdater.h"
#include "../loggers/Logger.h"
#include "ExperimentEnvironment.h"

struct OnlineExperimentParameters{
    int seed;
};

class OnlineExperiment{
  public:
    OnlineExperiment(OnlineExperimentParameters * parameters){
      srand(parameters->seed);
    };
    ~OnlineExperiment(){};
    void add_logger(Logger* logger){loggers_.push_back(logger);}
    void add_end_logger(Logger* logger){end_loggers_.push_back(logger);}
    void set_online_data_updater(OnlineDataUpdater* updater){online_data_updater_ = updater; }
    void set_online_recommender(OnlineRecommender* recommender){recommender_ = recommender; }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){ recommender_data_iterator_ = recommender_data_iterator; }
    bool self_test(){
      bool ok = true;
      if(recommender_data_iterator_ == NULL){
        ok = false;
        cerr << "OnlineExperiment::recommender_data_iterator_ is not set." << endl;
      }
      if(recommender_ == NULL){
        ok = false;
        cerr << "OnlineExperiment::recommender_ is not set." << endl;
      }
      if(online_data_updater_ == NULL){
        ok = false;
        cerr << "OnlineExperiment::online_data_updater_ is not set." << endl;
      }
      if(loggers_.size() == 0){
        //ok = false; //That's not a fatal error.
        cerr << "WARNING: OnlineExperiment::loggers_ is empty." << endl;
      }
      return ok;
    }
    void run();
  private:
    ExperimentEnvironment experiment_environment_;
    RecommenderDataIterator* recommender_data_iterator_;
    OnlineRecommender* recommender_;
    OnlineDataUpdater* online_data_updater_;
    vector<Logger*> loggers_;
    vector<Logger*> end_loggers_;
};

#endif
