struct RandomChoosingCombinedModelExpertUpdaterParameters{
  %TypeHeaderCode
  #include "models/combination/RandomChoosingCombinedModelExpertUpdater.h"
  %End
  double eta;
  int top_k;
  std::string loss_type; //abs, dcg, mrr, other
};

class RandomChoosingCombinedModelExpertUpdater
  : public Updater,
  public Initializable,
  public NeedsExperimentEnvironment 
{
  %TypeHeaderCode
  #include "models/combination/RandomChoosingCombinedModelExpertUpdater.h"
  %End
public:
  RandomChoosingCombinedModelExpertUpdater(RandomChoosingCombinedModelExpertUpdaterParameters* params);
  ~RandomChoosingCombinedModelExpertUpdater();
  //void set_evaluators(vector<Evaluator*> evaluators); //TODO
  void set_model(RandomChoosingCombinedModel* model);
  void set_experiment_environment(ExperimentEnvironment* experiment_environment);
  void update(RecDat* rec_dat);
  bool self_test();
  protected:
  bool autocalled_initialize();
};
