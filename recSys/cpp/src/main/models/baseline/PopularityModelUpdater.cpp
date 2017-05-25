#include "PopularityModelUpdater.h"

void PopularityModelUpdater::update(RecDat* rec_dat){
  int item = rec_dat->item;
  model_->items_.increase(item);
  if (model_->items_.get(item) > model_->maxitem_) model_->maxitem_ = model_->items_.get(item); 
}
