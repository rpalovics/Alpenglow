#include "PersonalPopularityModelUpdater.h"

void PersonalPopularityModelUpdater::update(RecDat * rec_dat){
  PopularityModelUpdater::update(rec_dat);

  double currentValue = model->train_matrix_.get(rec_dat->user, rec_dat->item) + rec_dat->score;
  model->train_matrix_.update(rec_dat->user, rec_dat->item, currentValue);
  double best = model->get_userBestItemScore(rec_dat->user);
  if(currentValue > best){
    model->userBestItems_[rec_dat->user] = rec_dat->item;
  }
}
