#include "PopularityModelUpdater.h"

void PopularityModelUpdater::update(RecDat* rec_dat){
  int item = rec_dat->item;
  model_->item_freqs_.increase(item);
  if (model_->item_freqs_.get(item) > model_->maxfreq_) model_->maxfreq_ = model_->item_freqs_.get(item); 
}
