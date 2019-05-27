#ifndef OFFLINE_TOP_LIST_EVALUATOR_H
#define OFFLINE_TOP_LIST_EVALUATOR_H
#include <gtest/gtest_prod.h>
#include "OfflineEvaluator.h"
#include "../utils/PredictionCreator.h"
//TODO rename global toplist evaluator

struct OfflineTopListEvaluatorParameters {
  string file_name;
};

class OfflineTopListEvaluator : public OfflineEvaluator {
  public:
    OfflineTopListEvaluator(OfflineTopListEvaluatorParameters* params){
      ofs.open(params->file_name.c_str());
      prediction_creator_ = NULL;
    }
    void set_prediction_creator(PredictionCreator* prediction_creator){ prediction_creator_ = prediction_creator; }
    void evaluate(){
  RecDat rec_dat;
  vector<RecDat>* top_predictions = prediction_creator_->run(&rec_dat);
  for(uint ii=0; ii<top_predictions->size(); ii++){
    ofs << top_predictions->at(ii).user << " " << top_predictions->at(ii).item << " " << top_predictions->at(ii).score <<  endl; 
  } 
};
    bool self_test(){
      bool OK = OfflineEvaluator::self_test();
      if(prediction_creator_==NULL){ OK=false; cerr << "OfflineTopListEvaluator::prediction_creator is not set." << endl; }
      return OK;
    }
  private:
    PredictionCreator* prediction_creator_;
    ofstream  ofs;
};

#endif /* OFFLINE_TOP_LIST_EVALUATOR_H */
