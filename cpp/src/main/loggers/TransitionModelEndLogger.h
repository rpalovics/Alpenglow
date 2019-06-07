#ifndef TRANSITION_MODEL_END_LOGGER_H
#define TRANSITION_MODEL_END_LOGGER_H

#include <fstream>
#include <algorithm>
#include <gtest/gtest_prod.h>
#include "Logger.h"
#include "../models/baseline/TransitionProbabilityModel.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../utils/PopContainers.h"
using namespace std;

struct TransitionModelEndLoggerParameters {
  string log_file_name = ""; //output format: track len_of_list popularity track1 track2 ... track_max_length
  int max_length = 10;
};
class TransitionModelEndLogger
  : public Logger
  , public NeedsExperimentEnvironment
  , public Initializable
  {
  public:
    TransitionModelEndLogger(TransitionModelEndLoggerParameters* params){
      log_file_name_=params->log_file_name;
      max_length_=params->max_length;
    }
    void run(RecDat* rec_dat) override;
    void set_pop_container(PopContainer* pop_container){ pop_container_ = pop_container; }
    void set_model(TransitionProbabilityModel* model){ model_ = model; }
    bool self_test(){
      bool OK = Logger::self_test();
      if(model_==NULL){ OK=false; cerr << "TransitionModelEndLogger::model is not set." << endl; }
      if(pop_container_==NULL){ OK=false; cerr << "TransitionModelEndLogger::pop_container is not set." << endl; }
      if(log_file_name_==""){ OK=false; cerr << "TransitionModelEndLogger::log_file_name is not set." << endl; }
      return OK;
    }
  protected:
    bool autocalled_initialize() override {
      if (pop_container_==NULL){ 
        if (experiment_environment_ == NULL) return false;
        pop_container_=experiment_environment_->get_popularity_container();
      }
      return true;
    }
  private:
    void run_core(ostream& ofs);
    TransitionProbabilityModel* model_ = NULL;
    const PopContainer* pop_container_ = NULL;
    string log_file_name_;
    int max_length_;
  FRIEND_TEST(TestTransitionEndLogger, test);
};


#endif /* TRANSITION_MODEL_END_LOGGER_H */
