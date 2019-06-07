#ifndef TRANSITION_MODEL_LOGGER_H
#define TRANSITION_MODEL_LOGGER_H

#include <fstream>
#include <algorithm>
#include <numeric>
#include "Logger.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../models/baseline/TransitionProbabilityModel.h"
#include "../utils/PopContainers.h"
using namespace std;

//SIP_AUTOCONVERT

struct TransitionModelLoggerParameters {
  string toplist_length_logfile_basename = ""; //output format: track toplist_len
  string timeline_logfile_name = "";
  int period_length = 86400;
  int top_k = -1;
};
class TransitionModelLogger
  : public Logger
  , public NeedsExperimentEnvironment
  , public Initializable
  { //SIP_NODEFAULTCTORS
  public:
    TransitionModelLogger(TransitionModelLoggerParameters* params):
      period_length_(params->period_length)
    {
      toplist_length_logfile_basename_=params->toplist_length_logfile_basename;
      timeline_logfile_name_=params->timeline_logfile_name;
      top_k_=params->top_k;
    }
    void run(RecDat* rec_dat) override {
      int period_num = (int)rec_dat->time/period_length_;
      if(period_num!=last_period_num_){
        write_toplist_lengths(rec_dat);
        last_period_num_=period_num;
      }
      write_eval_statistics(rec_dat);
    }
    void set_pop_container(PopContainer* pop_container){ pop_container_ = pop_container; }
    void set_model(TransitionProbabilityModel* model){ model_ = model; }
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_ = train_matrix; }
    bool self_test(){
      bool OK = Logger::self_test();
      if(model_==NULL){ OK=false; cerr << "TransitionModelLogger::model is not set." << endl; }
      if(pop_container_==NULL){ OK=false; cerr << "TransitionModelLogger::pop_container is not set." << endl; }
      if(train_matrix_==NULL){ OK=false; cerr << "TransitionModelLogger::train_matrix is not set." << endl; }
      if(!timeline_file_.is_open()){ OK=false; }
      return OK;
    }
  protected:
    bool autocalled_initialize() override {
      if (pop_container_==NULL){
        if (experiment_environment_ == NULL) return false;
        pop_container_=experiment_environment_->get_popularity_container();
      }
      if (train_matrix_==NULL){
        if (experiment_environment_ == NULL) return false;
        train_matrix_=experiment_environment_->get_train_matrix();
      }
      if (top_k_<0){
        if (experiment_environment_ == NULL) return false;
        top_k_=experiment_environment_->get_top_k();
      }
      timeline_file_.open(timeline_logfile_name_);
      return true;
    }
  private:
    //params
    string toplist_length_logfile_basename_;
    string timeline_logfile_name_;
    const int period_length_;
    int top_k_;
    //state
    int last_period_num_ = 0;
    //other
    ofstream timeline_file_;
    TransitionProbabilityModel* model_ = NULL;
    const PopContainer* pop_container_ = NULL;
    const SpMatrix* train_matrix_ = NULL;
    void write_eval_statistics(RecDat* rec_dat);
    vector<pair<int,int>> compute_toplist(map<int,int>* actual_frequency_map, RecDat* rec_dat);
    void write_toplist_lengths(RecDat* rec_dat);
};


#endif /* TRANSITION_MODEL_LOGGER_H */
