#ifndef ONLINE_EXPERIMENT
#define ONLINE_EXPERIMENT


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>

#include "../utils/SpMatrix.h"
#include "../online_learners/OnlineLearner.h"
#include "../models/Model.h"
#include "../ranking/Ranking.h"
#include "../recommender_data/RecommenderData.h"
#include "../recommender_data/RecommenderDataIterator.h"
#include "../recommender_data/ShuffleIterator.h"
#include "../online_recommender/OnlineRecommender.h"
#include "../online_data_updater/OnlineDataUpdater.h"
#include "../utils/Util.h"
#include "../loggers/Logger.h"

struct OnlineExperimentParameters{
    //double min_time, max_time;
    int seed;
};

class OnlineExperiment{
  public:
    OnlineExperiment(OnlineExperimentParameters * parameters){
      //set_parameters(parameters);
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
        ok = false;
        cerr << "OnlineExperiment::loggers_ is empty." << endl;
      }
      return ok;
    }
    void run();
  protected:
    RecommenderDataIterator* recommender_data_iterator_;
    OnlineRecommender* recommender_;
    OnlineDataUpdater* online_data_updater_;
    vector<Logger*> loggers_;
    vector<Logger*> end_loggers_;
    //double min_time_, max_time_;
    //void set_parameters(OnlineExperimentParameters * parameters);
};

#endif
