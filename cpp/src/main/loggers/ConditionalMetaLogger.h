#ifndef CONDITIONAL_META_LOGGER_H
#define CONDITIONAL_META_LOGGER_H

//SIP_AUTOCONVERT

#include "Logger.h"
using namespace std;

class ConditionalMetaLogger : public Logger{
  public:
    void run(RecDat* recDat) override {
      if(should_run(recDat)){
        logger_->run(recDat);
      }
    }
    virtual bool should_run(RecDat* recDat)=0;
    virtual void set_logger(Logger* logger){
      logger_ = logger;
    }
    bool self_test(){
      bool OK = Logger::self_test();
      if(logger_ == NULL){
        OK = false;
        cerr << "ConditionalMetaLogger::logger_ not set" << endl;
      }
      return OK;
    }
  private:
    Logger *logger_;
};

#endif /* CONDITIONAL_META_LOGGER_H */
