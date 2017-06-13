class Logger : public Initializable{
  %TypeHeaderCode
  #include "loggers/Logger.h"
  %End
  public:
    Logger();
    virtual ~Logger();
    virtual void run(RecDat* rec_dat)=0;
    virtual bool init();
    bool self_test();
};