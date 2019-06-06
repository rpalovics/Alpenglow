
// class ToplistCombinationModelRankingScoreIterator : public RankingScoreIterator{
//  %TypeHeaderCode
//  #include "models/combination/ToplistCombinationModel.h"
//  %End
//public:
//  bool has_next(double bound);
//  pair<int, double> get_next();
//  void set_up(std::vector<pair<int,double>> toplist);
//  void reinit();
//  int unique_items_num();
//};

struct ToplistCombinationModelParameters{
  %TypeHeaderCode
  #include "models/combination/ToplistCombinationModel.h"
  %End
  int seed;
  int top_k;
};
class ToplistCombinationModel
 : public Model,
//   virtual public RankingScoreIteratorProvider,
   public Initializable,
   public NeedsExperimentEnvironment
{
  %TypeHeaderCode
  #include "models/combination/ToplistCombinationModel.h"
  %End
public:
  ToplistCombinationModel(ToplistCombinationModelParameters* params);
  void add_model(Model* model);
  bool self_test();
  void add(RecDat* rec_dat);
  double prediction(RecDat* rec_dat);
//  void write(ostream& file);
//  void read(istream& file);
//  RankingScoreIterator* get_ranking_score_iterator(int user);
  ~ToplistCombinationModel();
  void inject_wms_into(WMSUpdater* object);
  protected:
  bool autocalled_initialize();
};

