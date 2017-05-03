#ifndef LOGGER
#define LOGGER

#include "../recommender_data/RecommenderData.h"
using namespace std;

class Logger{
  public:
    Logger(){}
    virtual ~Logger(){}
    virtual void run(RecDat* recDat)=0;
    virtual void init(){};
    bool self_test(){
      bool OK = true;
      return OK;
    }
    bool selfTest(){
      return self_test();
    }
};


#endif
