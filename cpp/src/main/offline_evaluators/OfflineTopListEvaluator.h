#ifndef OFFLINE_TOP_LIST_EVALUATOR_H
#define OFFLINE_TOP_LIST_EVALUATOR_H
#include <gtest/gtest_prod.h>
#include "OfflineEvaluator.h"
#include "../utils/ToplistCreator.h"
//TODO rename global toplist evaluator

struct OfflineTopListEvaluatorParameters {
  string file_name;
};

class OfflineTopListEvaluator : public OfflineEvaluator {
  public:
    OfflineTopListEvaluator(OfflineTopListEvaluatorParameters* params){
      ofs.open(params->file_name.c_str());
    }
    void set_prediction_creator(ToplistCreator* prediction_creator){
      prediction_creator_ = prediction_creator;
    }
    void evaluate() override;
    bool self_test(){
      bool ok = OfflineEvaluator::self_test();
      if(prediction_creator_==NULL) ok=false;
      if(!ofs.is_open()) ok=false;
      return ok;
    }
  private:
    ToplistCreator* prediction_creator_ = NULL;
    ofstream ofs;
};

#endif /* OFFLINE_TOP_LIST_EVALUATOR_H */
