class ProceedingLogger : public Logger{
  %TypeHeaderCode
  #include "loggers/ProceedingLogger.h"
  %End
  public:
    ProceedingLogger();
    virtual ~ProceedingLogger();
    virtual void run(RecDat* recDat);
    virtual void init();
    void set_data_iterator(RecommenderDataIterator* recommender_data_iterator /Transfer/);
    bool self_test();
};