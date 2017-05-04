class Logger{
  %TypeHeaderCode
  #include "loggers/Logger.h"
  %End
  public:
    Logger();
    virtual ~Logger();
    virtual void run(RecDat* recDat)=0;
    virtual void init();
    bool selfTest();
};