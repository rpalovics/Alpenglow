
class GlobalRankingScoreIterator {
  %TypeHeaderCode
  #include "models/GlobalRankingScoreIterator.h"
  %End
public:
  virtual void run(RecDat* rec_dat);
  virtual void run(double time);
//  virtual std::vector<pair<int,double>>* get_global_items();
//  virtual std::vector<pair<int,double>>* get_global_users();
//  virtual std::vector<pair<int,double>>* get_personalized_items(int user);
//  virtual std::vector<pair<int,double>>* get_personalized_users(int item);
  virtual bool active(RecDat*);
  virtual ~GlobalRankingScoreIterator();
  bool self_test();
};

