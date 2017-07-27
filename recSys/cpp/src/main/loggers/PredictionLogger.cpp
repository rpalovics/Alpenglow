#include "PredictionLogger.h"

void PredictionLogger::set_parameters(PredictionLoggerParameters * params){}

void PredictionLogger::run(RecDat * rd){
  // cerr << "PredictionLogger::predict computes prediction." << endl;
  vector<RecDat>* top_predictions = prediction_creator->run(rd);
  for(uint ii=0; ii<top_predictions->size(); ii++){
    // ofs << rd->id << " " << (int)rd->time << " " << top_predictions->at(ii).user << " " << top_predictions->at(ii).item << " " << top_predictions->at(ii).score << endl; 
    predictions_.ids.push_back(rd->id);
    predictions_.times.push_back(rd->time);
    predictions_.users.push_back(top_predictions->at(ii).user);
    predictions_.items.push_back(top_predictions->at(ii).item);
    predictions_.ranks.push_back(ii+1);
    predictions_.scores.push_back(top_predictions->at(ii).score);
  }
}