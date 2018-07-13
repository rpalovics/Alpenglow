
class ConditionalMetaLogger : public Logger{
  %TypeHeaderCode
  #include "loggers/ConditionalMetaLogger.h"
  %End
  public:
    virtual void run(RecDat* recDat);
    virtual bool should_run(RecDat* recDat)=0;
    virtual void set_logger(Logger* logger);
    bool self_test();
};

