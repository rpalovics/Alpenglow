
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
  void add_model(Model* model);
  void set_experiment_environment(ExperimentEnvironment* experiment_environment);
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

