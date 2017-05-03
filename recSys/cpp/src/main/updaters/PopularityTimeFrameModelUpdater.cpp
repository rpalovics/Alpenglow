#include "PopularityTimeFrameModelUpdater.h"


void PopularityTimeFrameModelUpdater::update(RecDat * recDat){
  model -> timeFrameData.push_back(recDat);
  int item = recDat -> item;
  double time = recDat -> time;
  model -> items[item]++;
  if (model-> maxitem < model -> items[item]) model -> maxitem = model -> items[item];
  list<RecDat*>::iterator timeIt; 
  for(timeIt = model->timeFrameData.begin(); timeIt!=model->timeFrameData.end() and (*timeIt)->time < time-model->tau; timeIt++){
    model -> items[(*timeIt)->item]--;
  }
  model -> timeFrameData.erase(model->timeFrameData.begin(),timeIt);
}
