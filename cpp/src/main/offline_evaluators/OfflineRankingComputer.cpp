#include "OfflineRankingComputer.h"

OfflinePredictions OfflineRankingComputer::compute(){
  OfflinePredictions preds;
  for(auto user : *users_){
    RecDat rec_dat;
    rec_dat.user=user;
    vector<RecDat>* toplist = toplist_creator_->run(&rec_dat);
    RANK rank = 1;
    for(auto predicted_rd: *toplist){
      preds.users.push_back(user);
      preds.items.push_back(predicted_rd.item);
      preds.ranks.push_back(rank);

      rank++;
    }
  }
  return preds;
}