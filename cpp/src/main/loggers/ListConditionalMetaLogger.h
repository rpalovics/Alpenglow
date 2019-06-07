#ifndef LIST_CONDITIONAL_META_LOGGER_H
#define LIST_CONDITIONAL_META_LOGGER_H

//SIP_AUTOCONVERT

#include "ConditionalMetaLogger.h"
#include <vector>

using namespace std;

struct ListConditionalMetaLoggerParameters {
  vector<int> should_run_vector;
};

class ListConditionalMetaLogger : public ConditionalMetaLogger{
  public:
    ListConditionalMetaLogger(ListConditionalMetaLoggerParameters* parameters){
      should_run_vector_ = parameters->should_run_vector;
    }
    virtual bool should_run(RecDat* recDat) override;
  protected:
    vector<int> should_run_vector_;
    int position=0;
};

#endif /* LIST_CONDITIONAL_META_LOGGER_H */
