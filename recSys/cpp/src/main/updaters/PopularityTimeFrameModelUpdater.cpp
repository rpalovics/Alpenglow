#include "PopularityTimeFrameModelUpdater.h"


void PopularityTimeFrameModelUpdater::update(RecDat * recDat){
  model -> time_frame_data.push_back(recDat);
  int item = recDat -> item;
  double time = recDat -> time;
  model -> items[item]++;
  if (model-> maxitem < model -> items[item]) model -> maxitem = model -> items[item];
  list<RecDat*>::iterator timeIt; 
  for(timeIt = model->time_frame_data.begin(); timeIt!=model->time_frame_data.end() and (*timeIt)->time < time-model->tau; timeIt++){
    model -> items[(*timeIt)->item]--;
  }
  model -> time_frame_data.erase(model->time_frame_data.begin(),timeIt);
}
