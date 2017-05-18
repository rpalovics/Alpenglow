#include "PersonalPopularityModelUpdater.h"

void PersonalPopularityModelUpdater::update(RecDat * rec_dat){
  PopularityModelUpdater::update(rec_dat);

  double current_value = model->train_matrix_.get(rec_dat->user, rec_dat->item) + rec_dat->score;
  model->train_matrix_.update(rec_dat->user, rec_dat->item, current_value);
  double best = model->get_user_best_item_score(rec_dat->user);
  if(current_value > best){
    model->user_best_items_[rec_dat->user] = rec_dat->item;
  }
}
