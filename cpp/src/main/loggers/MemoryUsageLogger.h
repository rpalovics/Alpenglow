#ifndef MEMORY_USAGE_LOGGER_H
#define MEMORY_USAGE_LOGGER_H

//SIP_AUTOCONVERT

#include <sys/resource.h>
#include <iostream>
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../recommender_data/RecommenderDataIterator.h"
#include "Logger.h"

using namespace std;

class MemoryUsageLogger : public Logger, public Initializable, public NeedsExperimentEnvironment{
  public:
    void run(RecDat* rec_dat) override {
      int counter = recommender_data_iterator_->get_counter();
      if(counter % frequency_ == 0 or counter == size_-1){
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        cerr << "mem usage: " << usage.ru_maxrss << endl;
      }
    }
    void set_data_iterator(RecommenderDataIterator* recommender_data_iterator){
      recommender_data_iterator_ = recommender_data_iterator;
    }
    bool self_test(){
      bool OK = Logger::self_test();
      if(recommender_data_iterator_==NULL){ OK=false; cerr << "MemoryUsageLogger::recommender_data_iterator_ is not set." << endl; }
      return OK;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL){ 
        if (experiment_environment_ == NULL) return false;
        recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator();
      }
      if(!recommender_data_iterator_->is_initialized()){
        return false;
      }
      size_=recommender_data_iterator_->size();
      frequency_=size_/100+1;
      return true;
    }
  private:
    const RecommenderDataIterator* recommender_data_iterator_ = NULL;
    int frequency_;
    int size_;
};

#endif /* MEMORY_USAGE_LOGGER_H */
