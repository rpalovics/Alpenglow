
// class WhitelistFilterRankingScoreIterator : public RankingScoreIterator {
//  %TypeHeaderCode
//  #include "filters/WhitelistFilter2ModelAdapter.h"
//  %End
//public:
//  WhitelistFilterRankingScoreIterator();
//  bool has_next(double lower_bound);
//  pair<int,double> get_next();
//  int unique_items_num();
//  void reset(int user, std::vector<int> whitelist);
//  void set_model(Model* model);
//  bool self_test();
//};

class WhitelistFilter2ModelAdapter
  : public Model
//  , public RankingScoreIteratorProvider
{
  %TypeHeaderCode
  #include "filters/WhitelistFilter2ModelAdapter.h"
  %End
public:
  double prediction(RecDat* rec_dat);
//  RankingScoreIterator* get_ranking_score_iterator(int user);
  void set_whitelist_filter(WhitelistFilter* whitelist_filter);
  void set_model(Model* model);
  bool self_test();
};

