#include "PersonalPopularityModel.h"

double PersonalPopularityModel::get_user_highest_personal_pop_item_score(int user){
  auto item = user_personal_pop_items_.find(user);
  if(item == user_personal_pop_items_.end()){
    return 0;
  } else {
    return personal_popularities_.get(user, item->second);
  }
}

void PersonalPopularityModel::add(RecDat* rec_dat){
  if(personal_popularities_.get(rec_dat->user, rec_dat->item) == 0){
    personal_popularities_.insert(rec_dat->user, rec_dat->item, 0);
  }
}

double PersonalPopularityModel::prediction(RecDat* rec_dat){
  double personal_pop = personal_popularities_.get(rec_dat->user, rec_dat->item);
  double highest_personal_pop = get_user_highest_personal_pop_item_score(rec_dat->user);
  double global_pop = global_popularities_.get(rec_dat->item);
  return log(personal_pop+global_pop+1)/log(highest_personal_pop+2);
}
