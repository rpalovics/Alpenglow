
// class NearestNeighborModelRankingScoreIterator : public RankingScoreIterator {
//  %TypeHeaderCode
//  #include "models/baseline/NearestNeighborModel.h"
//  %End
//public:
//  bool has_next(double bound);
//  pair<int, double> get_next();
//  void set_up(std::vector<SCORE>& predictions, std::vector<ITEM>& active_items);
//  void reinit();
//  int unique_items_num();
//};
struct NearestNeighborModelParameters {
  %TypeHeaderCode
  #include "models/baseline/NearestNeighborModel.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.NearestNeighborModel`. See documentation there.
  %End
  double gamma;
  std::string norm;
  std::string direction;
  double gamma_threshold;
  int num_of_neighbors;
};
class NearestNeighborModel
  : public Model
//  , virtual public RankingScoreIteratorProvider
{
  %TypeHeaderCode
  #include "models/baseline/NearestNeighborModel.h"
  %End
public:
  NearestNeighborModel(NearestNeighborModelParameters* params);
  double prediction(RecDat* rec_dat);
//  RankingScoreIterator* get_ranking_score_iterator(int user);
  bool self_test();

};

