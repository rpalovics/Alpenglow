struct ToplistCreatorParameters{
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  int top_k;
  int exclude_known;
};
class ToplistCreator : public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  public:
    ToplistCreator(ToplistCreatorParameters * params);
    virtual ~ToplistCreator();
    virtual std::vector<RecDat>* run(RecDat * rec_dat)=0; 
    void set_model(Model* model_);
    void set_filter(ModelFilter* filter_);
    void set_train_matrix(SpMatrix *train_matrix_);
    void set_experiment_environment(ExperimentEnvironment*);
    virtual bool self_test();
  protected:
    bool autocalled_initialize();
};

struct ToplistCreatorGlobalParameters : public ToplistCreatorParameters{
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  int initial_threshold;
};

class ToplistCreatorGlobal: public ToplistCreator{
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  public:
    ToplistCreatorGlobal(ToplistCreatorGlobalParameters* params);
    virtual ~ToplistCreatorGlobal();
    std::vector<RecDat>* run(RecDat* rec_dat);
    bool self_test();
};

struct ToplistCreatorPersonalizedParameters : public ToplistCreatorParameters{
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
};
class ToplistCreatorPersonalized: public ToplistCreator{
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  public:
    ToplistCreatorPersonalized(ToplistCreatorParameters * params);
    virtual ~ToplistCreatorPersonalized();
    std::vector<RecDat>* run(RecDat * rec_dat);
    bool self_test();
  protected:
    bool autocalled_initialize();
};
