
struct ListConditionalMetaLoggerParameters{
  %TypeHeaderCode
  #include "loggers/ListConditionalMetaLogger.h"
  %End
  std::vector<int> should_run_vector;
};

class ListConditionalMetaLogger : public ConditionalMetaLogger{
  %TypeHeaderCode
  #include "loggers/ListConditionalMetaLogger.h"
  %End
  public:
    ListConditionalMetaLogger(ListConditionalMetaLoggerParameters* parameters);
    virtual bool should_run(RecDat* recDat);
};

