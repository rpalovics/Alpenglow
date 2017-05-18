#include "PersonalPopularityModel.h"

double PersonalPopularityModel::get_userBestItemScore(int user){
  auto item = userBestItems_.find(user);
  if(item == userBestItems_.end()){
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
  double itemScore = train_matrix_.get(rec_dat->user, rec_dat->item);
  double bestScore = get_userBestItemScore(rec_dat->user);
  double popModelPrediction = PopularityModel::prediction(rec_dat);
  return log(itemScore+popModelPrediction+1)/log(bestScore+2);
}
