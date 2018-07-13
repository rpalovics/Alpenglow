
// class TransitionProbabilityModelRankingScoreIterator : public RankingScoreIterator{
//  %TypeHeaderCode
//  #include "models/baseline/TransitionProbabilityModel.h"
//  %End
//public:
//  bool has_next(double bound);
//  pair<int, double> get_next();
//  void set_up(map<int,int>* actual_frequency_map);
//  void reinit();
//  int unique_items_num();
//};

class TransitionProbabilityModel
  : public Model
//  , virtual public RankingScoreIteratorProvider
{
  %TypeHeaderCode
  #include "models/baseline/TransitionProbabilityModel.h"
  %End
  public:
    double prediction(RecDat* rec_dat);
//    RankingScoreIterator* get_ranking_score_iterator(int user);
    void clear();
    bool self_test();
};

