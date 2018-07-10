#include "PredictionLogger.h"


void PredictionLogger::run(RecDat* rd){
  vector<RecDat>* top_predictions = prediction_creator->run(rd);
  for(uint ii=0; ii<top_predictions->size(); ii++){
    predictions_.ids.push_back(rd->id);
    predictions_.times.push_back(rd->time);
    predictions_.users.push_back(top_predictions->at(ii).user);
    predictions_.items.push_back(top_predictions->at(ii).item);
    predictions_.ranks.push_back(ii+1);
    predictions_.scores.push_back(top_predictions->at(ii).score);
  }
}
