class ConditionalMetaLogger : public Logger, public Initializable{
  %TypeHeaderCode
  #include "loggers/ConditionalMetaLogger.h"
  %End
  public:
    ConditionalMetaLogger();
    virtual ~ConditionalMetaLogger();
    virtual void run(RecDat* recDat);
    virtual bool should_run(RecDat* recDat)=0;
    virtual bool init();
    virtual void set_logger(Logger* logger);
    bool self_test();
};
