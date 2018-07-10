#ifndef PROCEEDING_LOGGER
#define PROCEEDING_LOGGER

//SIP_AUTOCONVERT

#include <iostream>
#include <iomanip>
#include "../recommender_data/RecommenderDataIterator.h"
#include "../general_interfaces/Initializable.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "Logger.h"
using namespace std;

class ProceedingLogger : public Logger, public Initializable, public NeedsExperimentEnvironment {
  public:
    void run(RecDat* rec_dat) override {
      int counter = recommender_data_iterator_->get_counter();
      if(counter % frequency_ == 0){
        cerr << setprecision(4) << int((double)counter/(double)size_ * 100) <<"%-";
      }
      if(counter == size_-1){
        cerr << "OK" << endl;
      }
    }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override { experiment_environment_=experiment_environment; }
    void set_data_iterator(RecommenderDataIterator* recommender_data_iterator){recommender_data_iterator_ = recommender_data_iterator; }
    bool self_test(){
      bool OK = Logger::self_test();
      if(recommender_data_iterator_==NULL){ OK=false; cerr << "ProceedingLogger::recommender_data_iterator_ is not set." << endl; }
      return OK;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL){ recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator(); }
      if(!recommender_data_iterator_->is_initialized()){
        return false;
      }
      size_=recommender_data_iterator_->size();
      frequency_=size_/100+1;
      return true;
    }
  private:
    ExperimentEnvironment* experiment_environment_ = NULL;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    int frequency_;
    int size_;
};

#endif
