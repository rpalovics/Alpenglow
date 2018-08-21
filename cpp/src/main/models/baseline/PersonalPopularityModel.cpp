#include "PersonalPopularityModel.h"

double PersonalPopularityModel::get_user_highest_personal_pop_item_score(int user){
  auto item = user_personal_pop_items_.find(user);
  if(item == user_personal_pop_items_.end()){
    return 0;
  } else {
    return personal_popularities_.get(user, item->second);
  }
}


double PersonalPopularityModel::prediction(RecDat* rec_dat){
  double personal_pop = personal_popularities_.get(rec_dat->user, rec_dat->item);
  double highest_personal_pop = get_user_highest_personal_pop_item_score(rec_dat->user);
  double global_pop = global_popularities_.get(rec_dat->item);
  double global_score = 0;
  if(global_pop != 0){
    global_score = log(global_popularities_.get(rec_dat->item)+1)/log(highest_pop_+1);
  }
  return log(personal_pop+global_score+1)/log(highest_personal_pop+2);
}
