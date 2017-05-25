#include "PopularityTimeFrameModelUpdater.h"


void PopularityTimeFrameModelUpdater::update(RecDat * rec_dat){
  model -> time_frame_data.push_back(rec_dat);
  int item = rec_dat -> item;
  double time = rec_dat -> time;
  model -> items[item]++;
  if (model-> maxitem < model -> items[item]) model -> maxitem = model -> items[item];
  list<RecDat*>::iterator time_it; 
  for(time_it = model->time_frame_data.begin(); time_it!=model->time_frame_data.end() and (*time_it)->time < time-model->tau; time_it++){
    model -> items[(*time_it)->item]--;
  }
  model -> time_frame_data.erase(model->time_frame_data.begin(),time_it);
}
