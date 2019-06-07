#include "OfflineTopListEvaluator.h"

void OfflineTopListEvaluator::evaluate(){
  RecDat rec_dat;
  vector<RecDat>* top_predictions = prediction_creator_->run(&rec_dat);
  for(uint ii=0; ii<top_predictions->size(); ii++){
    ofs << top_predictions->at(ii).user << " " << top_predictions->at(ii).item
        << " " << top_predictions->at(ii).score <<  endl; 
  } 
}
