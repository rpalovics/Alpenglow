
class GlobalRankingScoreIterator  /Abstract/{
  %TypeHeaderCode
  #include "models/GlobalRankingScoreIterator.h"
  %End
public:
  virtual void run(RecDat* rec_dat);
//  virtual std::vector<pair<int,double>>* get_global_items()=0;
//  virtual std::vector<pair<int,double>>* get_global_users()=0;
  virtual ~GlobalRankingScoreIterator();
  bool self_test();
};

