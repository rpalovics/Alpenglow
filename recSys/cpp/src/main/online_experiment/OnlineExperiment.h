#ifndef ONLINE_EXPERIMENT
#define ONLINE_EXPERIMENT

#include "../recommender_data/RecommenderDataIterator.h"
#include "../online_learners/OnlineLearner.h"
#include "../general_interfaces/Updater.h"
#include "../loggers/Logger.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"
#include "ExperimentEnvironment.h"


class OnlineExperiment{
  public:
    OnlineExperiment(OnlineExperimentParameters* parameters){
      srand(parameters->random_seed);
      experiment_environment_.set_parameters(parameters);
    };
    ~OnlineExperiment(){};
    void add_logger(Logger* logger){loggers_.push_back(logger);}
    void add_end_logger(Logger* logger){end_loggers_.push_back(logger);}
    void add_learner(OnlineLearner* learner){ learners_.push_back(learner); }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
      recommender_data_iterator_ = recommender_data_iterator;
      experiment_environment_.set_recommender_data_iterator(recommender_data_iterator);
    }
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
    void inject_experiment_environment_into(INeedExperimentEnvironment *object){
      object->set_experiment_environment(&experiment_environment_);
    }
  private:
    ExperimentEnvironment experiment_environment_;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    vector<OnlineLearner*> learners_;
    vector<Logger*> loggers_;
    vector<Logger*> end_loggers_;
};

#endif
