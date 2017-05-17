#include "PersonalPopularityModelUpdater.h"

void PersonalPopularityModelUpdater::update(RecDat * recDat){
  PopularityModelUpdater::update(recDat);

  double currentValue = model->train_matrix_.get(recDat->user, recDat->item) + recDat->score;
  model->train_matrix_.update(recDat->user, recDat->item, currentValue);
  double best = model->getUserBestItemScore(recDat->user);
  if(currentValue > best){
    model->userBestItems_[recDat->user] = recDat->item;
  }
}
