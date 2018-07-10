#ifndef LOGGER
#define LOGGER

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"
using namespace std;

class Logger{
  public:
    Logger(){}
    virtual ~Logger(){}
    virtual void run(RecDat* rec_dat)=0;
    bool self_test(){
      bool OK = true;
      return OK;
    }
};


#endif
