#include "ListConditionalMetaLogger.h"

bool ListConditionalMetaLogger::should_run(RecDat* recDat){
  bool should = should_run_vector_.at(position);
  position+=1;
  return should;
}
