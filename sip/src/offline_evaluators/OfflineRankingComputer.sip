struct OfflineRankingComputerParameters{
  %TypeHeaderCode
  #include "offline_evaluators/OfflineRankingComputer.h"
  %End
  int top_k;
};

struct OfflinePredictions{
  %TypeHeaderCode
  #include "offline_evaluators/OfflineRankingComputer.h"
  %End
  std::vector<int> users;
  std::vector<int> items;
  std::vector<int> ranks;
};

class OfflineRankingComputer{
  %TypeHeaderCode
  #include "offline_evaluators/OfflineRankingComputer.h"
  %End
  public:
    OfflineRankingComputer(OfflineRankingComputerParameters *params);

    void set_toplist_creator(ToplistCreatorPersonalized* toplist_creator);

    void set_items(std::vector<int> *items);
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End
    void set_users(std::vector<int> *users);
    void set_users(VectorInt);
    %MethodCode
        sipCpp->set_users(&(a0->vec));
    %End
    OfflinePredictions compute();
};
