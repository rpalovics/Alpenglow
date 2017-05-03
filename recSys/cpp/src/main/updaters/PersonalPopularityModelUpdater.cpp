#include "PersonalPopularityModelUpdater.h"

void PersonalPopularityModelUpdater::update(RecDat * recDat){
  PopularityModelUpdater::update(recDat);

  double currentValue = model->trainMatrix_.get(recDat->user, recDat->item) + recDat->score;
  model->trainMatrix_.update(recDat->user, recDat->item, currentValue);
  double best = model->getUserBestItemScore(recDat->user);
  if(currentValue > best){
    model->userBestItems_[recDat->user] = recDat->item;
  }
}
