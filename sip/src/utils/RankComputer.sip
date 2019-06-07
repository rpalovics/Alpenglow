struct RankComputerParameters{
  %TypeHeaderCode
  #include "utils/RankComputer.h"
  %End
  int top_k;
  int random_seed;
};
class RankComputer : public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "utils/RankComputer.h"
  %End
  public:
    RankComputer(RankComputerParameters * parameters);
    ~RankComputer();
    void set_model(Model* model);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_top_pop_container(TopPopContainer* popularity_sorted_container);
    void set_model_filter(ModelFilter* model_filter);
    bool self_test();
    void set_experiment_environment(ExperimentEnvironment* ee);
    int get_rank(RecDat*);
  protected:
    bool autocalled_initialize();
};
