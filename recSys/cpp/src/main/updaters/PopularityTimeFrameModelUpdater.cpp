#include "PopularityTimeFrameModelUpdater.h"


void PopularityTimeFrameModelUpdater::update(RecDat * rec_dat){
  model -> time_frame_data.push_back(rec_dat);
  int item = rec_dat -> item;
  double time = rec_dat -> time;
  model -> items[item]++;
  if (model-> maxitem < model -> items[item]) model -> maxitem = model -> items[item];
  list<RecDat*>::iterator timeIt; 
  for(timeIt = model->time_frame_data.begin(); timeIt!=model->time_frame_data.end() and (*timeIt)->time < time-model->tau; timeIt++){
    model -> items[(*timeIt)->item]--;
  }
  model -> time_frame_data.erase(model->time_frame_data.begin(),timeIt);
}
