#ifndef CONDITIONAL_META_LOGGER
#define CONDITIONAL_META_LOGGER

#include "Logger.h"
using namespace std;

class ConditionalMetaLogger : public Logger, public Initializable{
  public:
    ConditionalMetaLogger(){}
    virtual ~ConditionalMetaLogger(){}
    virtual void run(RecDat* recDat){
      if(should_run(recDat)){
        logger_->run(recDat);
      }
    };
    virtual bool should_run(RecDat* recDat)=0;
    virtual void set_logger(Logger* logger){
      logger_ = logger;
    }
    bool self_test(){
      bool OK = true;
      if(logger_ == NULL){
        OK = false;
        cerr << "ConditionalMetaLogger::logger_ not set" << endl;
      }
      return OK;
    }
  protected:
   bool autocalled_initialize() override { //TODO remove unnecessary Initializable
     return true;
   }
  private:
    Logger *logger_;
};

#endif
