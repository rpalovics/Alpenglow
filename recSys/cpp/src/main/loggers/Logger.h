#ifndef LOGGER
#define LOGGER

#include "../recommender_data/RecommenderData.h"
using namespace std;

class Logger : public Initializable{
  public:
    Logger(){}
    virtual ~Logger(){}
    virtual void run(RecDat* rec_dat)=0;
    virtual bool init(){return true;};
    bool self_test(){
      bool OK = true;
      return OK;
    }
};


#endif
