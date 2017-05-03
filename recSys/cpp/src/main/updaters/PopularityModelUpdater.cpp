#include "PopularityModelUpdater.h"

void PopularityModelUpdater::update(RecDat * recDat){
  int item = recDat -> item;
  model->items.increase(item);
  if (model->items.get(item) > model->maxitem) model->maxitem = model->items.get(item); 
}
