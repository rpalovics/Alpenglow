#ifndef MEMORY_USAGE_LOGGER
#define MEMORY_USAGE_LOGGER

#include <sys/resource.h>
#include <iostream>
#include "../recommender_data/RecommenderDataIterator.h"
#include "Logger.h"

using namespace std;

class MemoryUsageLogger : public Logger{
  public:
    MemoryUsageLogger(){
      recommender_data_iterator_=NULL;
    }
    virtual ~MemoryUsageLogger(){}
    virtual void run(RecDat* rec_dat){
      int counter = recommender_data_iterator_->get_counter();
      if(counter % frequency_ == 0 or counter == size_-1){
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        cerr << "mem usage: " << usage.ru_maxrss << endl;
      }
    }
    virtual bool init(){
      if(!recommender_data_iterator_->is_initialized()){
        return false;
      }
      size_=recommender_data_iterator_->size();
      frequency_=size_/100+1;
      return true;
    }
    void set_data_iterator(RecommenderDataIterator* recommender_data_iterator){recommender_data_iterator_ = recommender_data_iterator; }
    bool self_test(){
      bool OK = Logger::self_test();
      if(recommender_data_iterator_==NULL){ OK=false; cerr << "MemoryUsageLogger::recommender_data_iterator_ is not set." << endl; }
      return OK;
    }
  private:
    RecommenderDataIterator* recommender_data_iterator_;
    int frequency_;
    int size_;
};

#endif
