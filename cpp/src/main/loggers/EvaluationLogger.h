#ifndef EVALUATION_LOGGER_H
#define EVALUATION_LOGGER_H

#include <fstream>
#include <iostream>
#include <cstring>
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"
#include "../models/Model.h"
#include "Logger.h"
#include "../recommender_data/RecommenderDataIterator.h"

//TODO
//kiertekeles mse eseten negativ mintakra is
//kiertekeles dcg-vel

struct EvaluationLoggerParameters {
  string file_name;
  int timeframe;
  string mode;
  string error_type;
};
class EvaluationLogger : public Logger, public NeedsExperimentEnvironment, public Initializable{
  public:
    EvaluationLogger(EvaluationLoggerParameters* params){
      file_name_ = params->file_name;
      output_file_.open(file_name_.c_str());
      timeframe_ = params->timeframe;
      mode_ = params->mode;
      error_type_ = params->error_type;
    }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
      recommender_data_iterator_ = recommender_data_iterator;
    }
    void set_model(Model* model){
      model_ = model;
    }
    void run(RecDat* rec_dat) override {
      double error = compute_avg_error_on_timeframe(rec_dat);
      write_avg_error_into_file(rec_dat, error);
    }
    bool self_test(){
      bool ok = Logger::self_test();
      if(recommender_data_iterator_==NULL) ok=false;
      if(model_==NULL) ok=false;
      //TODO tests:
      //output file is ok
      //mode is in iteration, timeframe, write out meanings
      //error type is in mse, ndcg, write out meanings
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL){
        if (experiment_environment_ == NULL) return false;
        recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator();
      }
      return true;
    }
  private:
    //parameters
    string file_name_;
    int timeframe_;
    string mode_;
    string error_type_;
    ofstream output_file_; 
    Model* model_ = NULL;
    const RecommenderDataIterator* recommender_data_iterator_ = NULL;
    //state
    int end_of_timeframe_;
    int beginning_of_timeframe_;
    double compute_avg_error_on_timeframe(RecDat* rec_dat){
      int direction = (timeframe_>0) ? 1 : -1;
      int index = recommender_data_iterator_->get_counter(); //TODO docs: cant evaluate far future...
      double error = 0;
      int number_of_samples = 0;
      while(index>0 && index<recommender_data_iterator_->size()){
        RecDat* sample = recommender_data_iterator_->get_future(index); //logger may evaluate future
        if(abs(sample->time-rec_dat->time)>abs(timeframe_)) break; //TODO may be slow
        error+=compute_error_for_sample(sample);
        index+=direction;
        number_of_samples++;
      }
      //cerr << "error" << error_type_ << " " << error << endl;
      error/=number_of_samples;
      return error;
    }
    double compute_error_for_sample(RecDat* rec_dat){
      if(strcmp(error_type_.c_str(),"mse")==0){
        return rec_dat->score - model_->prediction(rec_dat);
      } else { //dcg
        return 0; //TODO
      }
    }
    void write_avg_error_into_file(RecDat* rec_dat, double error){
      output_file_ << rec_dat->time << " " << error << endl;
    }
};


#endif /* EVALUATION_LOGGER_H */
