#include "PopularityTimeFrameModelUpdater.h"


void PopularityTimeFrameModelUpdater::update(RecDat* rec_dat){
  //increase (same as pop)
  int item = rec_dat->item;
  model_->item_freqs_.increase(item);
  if (model_->item_freqs_.get(item) > model_->maxfreq_) model_->maxfreq_ = model_->item_freqs_.get(item); 

  //decrease (extra part)
  time_frame_data_.push(rec_dat);
  double time = rec_dat -> time;
  while(!time_frame_data_.empty()){
    RecDat* hist_rec_dat = time_frame_data_.front();
    if(hist_rec_dat->time>=time-tau_) break;
    model_->item_freqs_.reduce(hist_rec_dat->item);
    time_frame_data_.pop();
  }
}
