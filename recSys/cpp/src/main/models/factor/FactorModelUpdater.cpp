#include "FactorModelUpdater.h"

void FactorModelUpdater::update(RecDat* rec_dat){
  if(model_->user_recency_ != NULL) model_->user_recency_->update(rec_dat->user,rec_dat->time);
  if(model_->item_recency_ != NULL) model_->item_recency_->update(rec_dat->item,rec_dat->time);
  model_->notify_observers();
}
