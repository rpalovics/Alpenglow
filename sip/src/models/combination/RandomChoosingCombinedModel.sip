
class RandomChoosingCombinedModel
 : public Model,
//   virtual public RankingScoreIteratorProvider,
   public Initializable,
   public NeedsExperimentEnvironment
{
  %TypeHeaderCode
  #include "models/combination/RandomChoosingCombinedModel.h"
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
  void inject_wms_into(WMSUpdater* object);
  protected:
  bool autocalled_initialize();
};

