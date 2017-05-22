#include "PersonalPopularityModel.h"

double PersonalPopularityModel::get_user_best_item_score(int user){
  auto item = user_best_items_.find(user);
  if(item == user_best_items_.end()){
    return 0;
  } else {
    return train_matrix_.get(user, item->second);
  }
}

void PersonalPopularityModel::add(RecDat * rec_dat){
  PopularityModel::add(rec_dat);
  if(train_matrix_.get(rec_dat->user, rec_dat->item) == 0){
    train_matrix_.insert(rec_dat->user, rec_dat->item, 0);
  }
}

double PersonalPopularityModel::prediction(RecDat * rec_dat){
  double item_score = train_matrix_.get(rec_dat->user, rec_dat->item);
  double best_score = get_user_best_item_score(rec_dat->user);
  double pop_model_prediction = PopularityModel::prediction(rec_dat);
  return log(item_score+pop_model_prediction+1)/log(best_score+2);
}
