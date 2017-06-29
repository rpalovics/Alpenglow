struct RankComputerParameters{
  %TypeHeaderCode
  #include "ranking/Ranking.h"
  %End
  int top_k;
  int random_seed;
};
class RankComputer : public INeedExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "ranking/Ranking.h"
  %End
  public:
    RankComputer(RankComputerParameters * parameters);
    ~RankComputer();
    void set_recommender(OnlineRecommender* recommender);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_top_pop_container(TopPopContainer* popularity_sorted_container);
    void set_model_filter(ModelFilter* model_filter);
    bool self_test();
    bool init();
    void set_experiment_environment(ExperimentEnvironment* ee);
    int get_rank(RecDat*);
};
