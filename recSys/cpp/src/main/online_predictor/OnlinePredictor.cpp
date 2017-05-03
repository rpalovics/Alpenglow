#include "OnlinePredictor.h"

void OnlinePredictor::setParameters(OnlinePredictorParameters * params){
  minTime = params->minTime;
  timeFrame = params->timeFrame;
  ofs.open(params->fileName.c_str());
  pastTimeFrame = 0;
  actualTimeFrame = 0;
  predictionCreator=NULL;
}

void OnlinePredictor::run(RecDat * recDat){
  if(doPredict(recDat)){
    cerr << "OnlinePredictor::predict computes prediction." << endl;
    vector<RecDat>* topPredictions = predictionCreator->run(recDat);
    for(uint ii=0; ii<topPredictions->size(); ii++){
      ofs << actualTimeFrame << " " << topPredictions->at(ii).user << " " << topPredictions->at(ii).item << " " << topPredictions->at(ii).score << endl; 
    } 
    cerr << "OnlinePredictor::predict done" << endl;
  }
}

bool OnlinePredictor::doPredict(RecDat * recDat){
  double actualTime = recDat->time;
  actualTimeFrame = (int)(actualTime - minTime)/(timeFrame);
  if(actualTime > minTime && actualTimeFrame !=pastTimeFrame){
    pastTimeFrame = actualTimeFrame;
    return true;
  }
  else return false;
}
