struct PredictionCreatorParameters{
  %TypeHeaderCode
  #include "utils/PredictionCreator.h"
  %End
  int top_k;
  int exclude_known;
};
class PredictionCreator : public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "utils/PredictionCreator.h"
  %End
  public:
    PredictionCreator(PredictionCreatorParameters * params);
    virtual ~PredictionCreator();
    virtual std::vector<RecDat>* run(RecDat * rec_dat)=0; 
    void set_model(Model* model_);
    void set_filter(ModelFilter* filter_);
    void set_train_matrix(SpMatrix *train_matrix_);
    void set_experiment_environment(ExperimentEnvironment*);
    virtual bool self_test();
  protected:
    bool autocalled_initialize();
};

struct PredictionCreatorGlobalParameters : public PredictionCreatorParameters{
  %TypeHeaderCode
  #include "utils/PredictionCreator.h"
  %End
  int initial_threshold;
};

class PredictionCreatorGlobal: public PredictionCreator{
  %TypeHeaderCode
  #include "utils/PredictionCreator.h"
  %End
  public:
    PredictionCreatorGlobal(PredictionCreatorGlobalParameters* params);
    virtual ~PredictionCreatorGlobal();
    std::vector<RecDat>* run(RecDat* rec_dat);
    bool self_test();
};

struct PredictionCreatorPersonalizedParameters : public PredictionCreatorParameters{
  %TypeHeaderCode
  #include "utils/PredictionCreator.h"
  %End
};
class PredictionCreatorPersonalized: public PredictionCreator{
  %TypeHeaderCode
  #include "utils/PredictionCreator.h"
  %End
  public:
    PredictionCreatorPersonalized(PredictionCreatorParameters * params);
    virtual ~PredictionCreatorPersonalized();
    std::vector<RecDat>* run(RecDat * rec_dat);
    bool self_test();
  protected:
    bool autocalled_initialize();
};
