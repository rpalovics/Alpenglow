#include "PersonalPopularityModel.h"

double PersonalPopularityModel::getUserBestItemScore(int user){
  auto item = userBestItems_.find(user);
  if(item == userBestItems_.end()){
    return 0;
  } else {
    return trainMatrix_.get(user, item->second);
  }
}

void PersonalPopularityModel::add(RecDat * recDat){
  PopularityModel::add(recDat);
  if(trainMatrix_.get(recDat->user, recDat->item) == 0){
    trainMatrix_.insert(recDat->user, recDat->item, 0);
  }
}

double PersonalPopularityModel::prediction(RecDat * recDat){
  double itemScore = trainMatrix_.get(recDat->user, recDat->item);
  double bestScore = getUserBestItemScore(recDat->user);
  double popModelPrediction = PopularityModel::prediction(recDat);
  return log(itemScore+popModelPrediction+1)/log(bestScore+2);
}
