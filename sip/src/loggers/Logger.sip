
class Logger{
  %TypeHeaderCode
  #include "loggers/Logger.h"
  %End
  public:
    Logger();
    virtual ~Logger();
    virtual void run(RecDat* rec_dat)=0;
    bool self_test();
};

