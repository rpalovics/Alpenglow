#ifndef PROCEEDING_LOGGER
#define PROCEEDING_LOGGER

#include <iostream>
#include <iomanip>
#include "../recommender_data/RecommenderDataIterator.h"
#include "Logger.h"
using namespace std;

class ProceedingLogger : public Logger{
  public:
    ProceedingLogger(){
      recommender_data_iterator_=NULL;
    }
    virtual ~ProceedingLogger(){}
    virtual void run(RecDat* recDat){
      int counter = recommender_data_iterator_->get_counter();
      if(counter % frequency_ == 0){
        cerr << setprecision(4) << (double)counter/(double)size_ * 100 <<"\%-";
      }
      if(counter == size_-1){
        cerr << "OK" << endl;
      }
    }
    virtual void init(){
      size_=recommender_data_iterator_->size();
      frequency_=size_/100+1;
    }
    void set_data_iterator(RecommenderDataIterator* recommender_data_iterator){recommender_data_iterator_ = recommender_data_iterator; }
    bool self_test(){
      bool OK = Logger::self_test();
      if(recommender_data_iterator_==NULL){ OK=false; cerr << "ProceedingLogger::recommender_data_iterator_ is not set." << endl; }
      return OK;
    }
  private:
    RecommenderDataIterator* recommender_data_iterator_;
    int frequency_;
    int size_;
};

#endif
