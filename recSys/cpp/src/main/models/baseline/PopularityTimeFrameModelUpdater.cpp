#include "PopularityTimeFrameModelUpdater.h"


void PopularityTimeFrameModelUpdater::update(RecDat* rec_dat){
  int item = rec_dat->item;
  model_->items_.increase(item);
  if (model_->items_.get(item) > model_->maxitem_) model_->maxitem_ = model_->items_.get(item); 
  time_frame_data_.push(rec_dat);
  double time = rec_dat -> time;
  while(!time_frame_data_.empty()){
    RecDat* hist_rec_dat = time_frame_data_.front();
    if(hist_rec_dat->time>=time-model_->tau_) break;
    model_->items_.reduce(hist_rec_dat->item);
    time_frame_data_.pop();
  }
}
