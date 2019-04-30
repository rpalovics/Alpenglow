#ifndef OFFLINE_RANKING_COMPUTER_H
#define OFFLINE_RANKING_COMPUTER_H

#include "../utils/PredictionCreator.h"

struct OfflineRankingComputerParameters {
  int top_k;
};

struct OfflinePredictions{
  vector<int> users;
  vector<int> items;
  vector<int> ranks;
};

class OfflineRankingComputer{
  public:
    OfflineRankingComputer(OfflineRankingComputerParameters *params){
      top_k_ = params->top_k;
    }

    void set_toplist_creator(PredictionCreatorPersonalized* toplist_creator){ toplist_creator_ = toplist_creator; }

    void set_items(vector<int> *items){items_ = items;}
    void set_users(vector<int> *users){users_ = users;}
    OfflinePredictions compute();
  private:
    int top_k_;
    vector<int> *items_;
    vector<int> *users_;
    PredictionCreatorPersonalized* toplist_creator_;
};

#endif /* OFFLINE_RANKING_COMPUTER_H */
